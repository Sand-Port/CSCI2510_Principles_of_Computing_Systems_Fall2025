#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char input_string[100];

    // Prompt user for a command
    printf("Enter a command: ");
    fgets(input_string, sizeof(input_string), stdin);

    // Strip newline character from input
    input_string[strcspn(input_string, "\n")] = '\0';

    // Declare variables
    int max_args = 15;
    int max_argv_size = max_args + 2; // one for argv[0], one for NULL terminator
    char* cmd;
    char* my_argv[max_argv_size];

    // Get first token (the command)
    cmd = strtok(input_string, " \t\n");
    if (cmd == NULL) {
        printf("No command entered.\n");
        return 1;
    }

    // Fill my_argv array
    my_argv[0] = cmd;
    int i = 1;
    char* res = strtok(NULL, " \t\n");
    while (res != NULL && i < max_argv_size - 1) {
        my_argv[i] = res;
        i++;
        res = strtok(NULL, " \t\n");
    }
    my_argv[i] = NULL;

    // Execute the command
    execvp(cmd, my_argv);

    // If execvp returns, there was an error
    perror("execvp failed");
    return 1;
}

