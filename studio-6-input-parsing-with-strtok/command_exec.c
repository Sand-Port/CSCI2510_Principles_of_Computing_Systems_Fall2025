#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char input_string[100];

    // Prompt user
    printf("Enter a command: ");
    fgets(input_string, sizeof(input_string), stdin);

    // Strip newline character from fgets
    input_string[strcspn(input_string, "\n")] = '\0';

    // Variables for later execvp use
    int max_args = 15;
    int max_argv_size = max_args + 2; // one for argv[0], one for null terminator
    char* cmd;
    char* my_argv[max_argv_size];

    // Get first token — this will be our command
    cmd = strtok(input_string, " \t\n");

    // For now, just confirm what was read
    if (cmd != NULL) {
        printf("Command: %s\n", cmd);
    } else {
        printf("No command entered.\n");
    }

    return 0;
}

