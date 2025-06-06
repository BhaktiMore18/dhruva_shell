#include <sys/types.h>//for pid_t
#include <sys/wait.h>//for waitpid(), WIFEXITED, WUNTRACED, WIFSIGNALED
#include <unistd.h> // for fork(), execvp()
#include <stdlib.h>  // for malloc(), realloc(), exit(), EXIT_SUCCESS
#include <stdio.h>   // for getchar(), fprintf(), printf(), stderr

#define DSH_RL_BUFSIZE 1024  // Default buffer size to start reading input
#define DSH_TOK_BUFSIZE 64  // Starting size for our array of tokens (arguments)
#define DSH_TOK_DELIM " \t\r\n\a"  // These characters will separate tokens (like spaces, tabs, newlines, etc.)

/*
 * This function launches a program.
 * It does this by forking the current process, and in the child process,
 * it replaces itself with the program we want to run using execvp().
 * 
 * args is a NULL-terminated array of strings (just like we got from dsh_split_line).
 * args[0] is the command (like "ls"), args[1], args[2], etc. are its arguments.
 */
int dsh_launch(char **args) {
    pid_t pid, wpid;  // pid: process ID of child, wpid: for waiting
    int status;       // to store the exit status of the child

    // Fork the current process: create a duplicate process
    pid = fork();

    if (pid == 0) {
        // This block runs in the child process

        /*
         * execvp replaces the current child process image with the command in args.
         * If it works, it doesn't return.
         * If it fails (e.g., command not found), it returns -1.
         */
        if (execvp(args[0], args) == -1) {
            perror("dsh");  // print error message if exec fails
        }

        // If exec failed, we manually exit the child process
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) {
        // fork() failed: maybe out of memory or too many processes
        perror("dsh");  // print the error message
    } 
    else {
        // This block runs in the parent process

        // Wait for the child process to finish
        // We use waitpid to wait specifically for the child we just created
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
            // We loop until the child either exits normally or is terminated by a signal
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;  // Returning 1 so that the shell continues running
}

/*
 * This function takes a full line of input (like: "ls -l /home")
 * and splits it into individual parts (tokens) like: ["ls", "-l", "/home"]
 * It returns an array of strings (char pointers), which will be passed to exec-like functions.
 */
char **dsh_split_line(char *line) {
    int bufsize = DSH_TOK_BUFSIZE;  // how many tokens we can store for now
    int position = 0;               // current position in the tokens array

    // Allocate memory for storing token pointers (char* array)
    char **tokens = malloc(bufsize * sizeof(char*));  
    char *token;  // to temporarily hold each token

    // Always check if malloc succeeded!
    if (!tokens) {
        fprintf(stderr, "dsh: allocation error\n");
        exit(EXIT_FAILURE);  // If we can’t allocate memory, exit the program
    }

    /*
     * strtok breaks the string `line` into tokens separated by delimiters.
     * It modifies the original string by inserting '\0' at the end of each token.
     * First call gives the first token, subsequent calls (with NULL) give the next ones.
     */
    token = strtok(line, DSH_TOK_DELIM);
    
    while (token != NULL) {
        tokens[position] = token;  // store the pointer to this token
        position++;  // move to the next slot in the tokens array

        // If we’ve run out of space in the tokens array, make it bigger!
        if (position >= bufsize) {
            bufsize += DSH_TOK_BUFSIZE;  // increase the buffer size
            tokens = realloc(tokens, bufsize * sizeof(char*));  // try to reallocate

            if (!tokens) {
                fprintf(stderr, "dsh: allocation error\n");
                exit(EXIT_FAILURE);  // again, check if reallocation failed
            }
        }

        // Get the next token (this will be NULL if we're done)
        token = strtok(NULL, DSH_TOK_DELIM);
    }

    // After the loop, we NULL-terminate the array so we know where it ends
    tokens[position] = NULL;
    return tokens;
}


/*
 * This function reads a full line of input from stdin (usually the terminal).
 * It keeps reading until the user presses Enter (newline) or Ctrl+D (EOF).
 * Since we don’t know in advance how long the input will be,
 * we dynamically grow the buffer if needed.
 */
char *dsh_read_line(void) {
    int bufsize = DSH_RL_BUFSIZE;  // initial size of our input buffer
    int position = 0;              // current position to insert next character
    char *buffer = malloc(sizeof(char) * bufsize);  // allocate memory
    int c;  // NOTE: using int here is **very important** to handle EOF correctly

    // Check if malloc failed
    if (!buffer) {
        fprintf(stderr, "dsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Read characters one by one until newline or EOF
    while (1) {
        c = getchar();  // reads one character from stdin

        // If we hit EOF (Ctrl+D) or user presses Enter, we stop reading
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';  // null-terminate the string
            return buffer;  // return the full line we read
        } else {
            buffer[position] = c;  // store the character
        }
        position++;  // move to the next position

        // If buffer is full, we need more space — reallocate it
        if (position >= bufsize) {
            bufsize += DSH_RL_BUFSIZE;  // increase buffer size
            buffer = realloc(buffer, bufsize);  // try to reallocate more memory

            // Check if realloc failed
            if (!buffer) {
                fprintf(stderr, "dsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}



/*
 * dsh_loop is the heart of our shell.
 * This loop keeps prompting the user, reading their input,
 * parsing it into commands and arguments, and executing it.
 * It runs over and over — like how bash or zsh works — until the user exits.
 */
void dsh_loop(void) {
    char *line;   // holds the line typed by the user
    char **args;  // holds the command + arguments (after splitting)
    int status;   // keeps track of whether we should continue or exit

    /*
     * A do-while loop ensures we run the shell at least once before checking status.
     * This is perfect for a shell, because we *want* it to run until told otherwise.
     */
    do {
        printf("dhruva > ");   // our prompt (you can customize this!)
        line = dsh_read_line();           // 1. Read: get user input
        args = dsh_split_line(line);      // 2. Parse: break it into command & args
        status = dsh_execute(args);       // 3. Execute: run the command

        // After executing, we free up the memory used by line and args
        free(line);
        free(args);

    } while (status);  // if dsh_execute returns 0, we exit
}


int main(int argc, char **argv) {
    // Entry point of the shell.
    // argc: number of arguments
    // argv: array of arguments (e.g., script name or flags)

    // Load config files, if any.
    // Useful if you want to support something like ~/.dshrc
    // You can parse it and set things like prompt or aliases.
    // (Not implemented yet, but leaving a comment for clarity.)

    // Run the main loop of the shell
    dsh_loop();
    // This is where we keep asking the user for commands,
    // read them, parse them, and execute them.
    // It runs until the user types 'exit' or something similar.

    // Cleanup and shutdown tasks
    // If you allocated memory or opened files, you should free/close them here.
    // For now, nothing to clean up, keeping it minimal and simple.

    return EXIT_SUCCESS;
    // Return a success code to the OS.
}
