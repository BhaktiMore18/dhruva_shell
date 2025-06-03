#include <stdlib.h>  // for malloc(), realloc(), exit(), EXIT_SUCCESS
#include <stdio.h>   // for getchar(), fprintf(), printf(), stderr

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
