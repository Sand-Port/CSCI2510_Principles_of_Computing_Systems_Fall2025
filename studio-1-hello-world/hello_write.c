// Sahand Darwish
// 2025-09-11
// This program prints a message to the console using the write() system call

#include <unistd.h>  // for write() and STDOUT_FILENO

int main(int argc, char* argv[]) {
    const char message[] = "Hello from write()!\n";
    write(STDOUT_FILENO, message, 200);  // 20 characters including the '\n'
    return 0;
}
