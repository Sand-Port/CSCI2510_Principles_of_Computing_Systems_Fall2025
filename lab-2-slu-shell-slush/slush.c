// SLUSH - The SLU SHELL
//
// This assignment reads user input and executes that input as a sequence of
// programs to execute.
//
// Program execution commands have the form:
//
// prog_n [args] ( ... prog_3 [args] ( prog_2 [args] ( prog_1 [args]
//
// For example, "grep slush ( sort -r ( ls -l -a" returns a reverse sorted list
// of all files in the current directory that contain the string "slush"
//
// This shell supports:
//   - Built-in command: cd dir
//   - Piped execution using '('
//   - Handling of Ctrl+C (SIGINT) and Ctrl+D (EOF)
//   - Error messages for invalid syntax or execution errors
//
// See the lab writeup for more details and requirements.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define MAX_LINE 256
#define MAX_ARGS 15
#define MAX_CMDS 10

// Stops cntrl C from terminating the terminal
void handle_sigint(int sig) {
    (void)sig;  // Suppress unused variable warning
    printf("\nslush> ");
    fflush(stdout);
}

// WE process arguments into an array so that we can now what to do in 
// what order
void parse_args(char* cmd, char** argv) {
    int argc = 0;
    char* token = strtok(cmd, " ");
    while (token != NULL && argc < MAX_ARGS) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;
}

// Recursive pipeline execution from right to left because thats how we
//we want our program to procress commands 
void execute_pipeline(char** cmds, int index, int input_fd) {
    if (index < 0) return;

    int pipefd[2];
    pid_t pid;

    pipe(pipefd);
    pid = fork();

    if (pid == 0) {
        // pid equaling 0 means that it's a child

        // Redirect stdin if input_fd is set
        if (input_fd != 0) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // WE want to send info to the next argument rather than stdout
        if (index > 0) {
            dup2(pipefd[1], STDOUT_FILENO);
        }

        // WE DONT LIKE UNUSED PIPES SO WE FREE THEM
        close(pipefd[0]); //BE FREE!!!
        close(pipefd[1]);

        // Parse
        char* argv[MAX_ARGS + 1];
        parse_args(cmds[index], argv);

        // No command? no Problem!!
        if (argv[0] == NULL) {
            fprintf(stderr, "Invalid null command\n");
            exit(1);
        }

        // We do not want to use built in cd command
        if (strcmp(argv[0], "cd") == 0) {
            fprintf(stderr, "cd: Not valid in pipeline\n");
            exit(1);
        }

        // Execute external command
        execvp(argv[0], argv);
        perror(argv[0]); // We do not want no commands!
        exit(1);
    } else {
        //this is if it is not a child and instead a parent

        waitpid(pid, NULL, 0); // Wait for child
        close(pipefd[1]);      // Close write end

        // Recurse if there are more commands
        if (index > 0) {
            execute_pipeline(cmds, index - 1, pipefd[0]);
        } else {
            close(pipefd[0]); // No more commands
        }
    }
}

int main() {
    char input[MAX_LINE];

    // Register signal handler for Ctrl+C
    signal(SIGINT, handle_sigint);

    while (1) {
        printf("slush> ");
        fflush(stdout);

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Our EOF opperant aka ctr D
            printf("\n");
            break;
        }

        // NO NEWLINE
        input[strcspn(input, "\n")] = '\0';
        // Ignore empty input
        if (strlen(input) == 0) continue;

        // Handle built-in "cd dir"
        if (strncmp(input, "cd ", 3) == 0) {
            char* dir = input + 3;
            if (chdir(dir) != 0) {
                perror("cd");
            }
            continue;
        }

        // split pipe by using  '(' as a landmark
        char* cmds[MAX_CMDS];
        int cmd_count = 0;

        char* token = strtok(input, "(");
        while (token != NULL && cmd_count < MAX_CMDS) {
            // WE dont like leading spaces
            while (*token == ' ') token++;

            cmds[cmd_count++] = token;
            token = strtok(NULL, "(");
        }

        // Making sure its intputted correctly
        for (int i = 0; i < cmd_count; i++) {
            if (strlen(cmds[i]) == 0) {
                fprintf(stderr, "Invalid null command\n");
                cmd_count = 0;
                break;
            }
        }

        // Executes pipeline if valid
        if (cmd_count > 0) {
            execute_pipeline(cmds, cmd_count - 1, 0);
        }
    }

    return 0;
}

