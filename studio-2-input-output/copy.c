#include <unistd.h>   // for read(), write(), STDIN_FILENO, STDOUT_FILENO

#define bufferSize 200

char buffer[bufferSize];

int main(void) {
    while (1) {
        ssize_t bytesRead = read(STDIN_FILENO, buffer, bufferSize);

        if (bytesRead == 0) {
            // End of file reached
            break;
        }

        // Write only the number of bytes actually read
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    return 0;
}

