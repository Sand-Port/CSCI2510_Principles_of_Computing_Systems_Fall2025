#include <stdio.h>
#include <string.h>

int main() {
    char input_string[100];

    printf("Enter some text: ");
    fgets(input_string, sizeof(input_string), stdin); // get user input

    // get first token separated by space
    char *token = strtok(input_string, " \t\n");

    // print first token
    if (token != NULL) {
        printf("First token: %s\n", token);
    } else {
        printf("No token found.\n");
    }

    return 0;
}

