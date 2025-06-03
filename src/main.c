#include <stdlib.h>  // for EXIT_SUCCESS

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
