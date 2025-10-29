#include <stdio.h>

int main() {
    char input_string[100]; // buffer to store user input

    printf("Enter some text: ");
    // read up to 99 characters or until newline
    if (fgets(input_string, sizeof(input_string), stdin) != NULL) {
        printf("You entered: %s\n", input_string);
    } else {
        printf("Error reading input.\n");
    }

    return 0;
}

