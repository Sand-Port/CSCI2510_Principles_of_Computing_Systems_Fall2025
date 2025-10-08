//Runlength compression in C
// Usage: ./rle <input file> <output file> <runlength> <mode>
//        Where mode=0 is compress and mode=1 is decompress
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_RUN 255

// Error helper
void die(const char *msg) {
    perror(msg);
    exit(-1);
}

void compressFile(const char *infile, const char *outfile, int k) {
    int fd_in = open(infile, O_RDONLY);
    if (fd_in < 0) die("open input");

    int fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_out < 0) die("open output");

    unsigned char prev[256], curr[256]; // up to 256-byte patterns
    ssize_t bytesRead = read(fd_in, prev, k);
    if (bytesRead < 0) die("read");

    if (bytesRead == 0) { // empty file
        close(fd_in);
        close(fd_out);
        return;
    }

    unsigned char count = 1;

    while (1) {
        bytesRead = read(fd_in, curr, k);
        if (bytesRead < 0) die("read");

        if (bytesRead == k && memcmp(prev, curr, k) == 0 && count < MAX_RUN) {
            count++;
        } else {
            // write (count + prev pattern)
            if (write(fd_out, &count, 1) != 1) die("write count");
            if (write(fd_out, prev, k) != k) die("write pattern");

            if (bytesRead != k) {
                // handle partial pattern or EOF
                if (bytesRead > 0) {
                    count = 1;
                    if (write(fd_out, &count, 1) != 1) die("write count");
                    if (write(fd_out, curr, bytesRead) != bytesRead) die("write last partial");
                }
                break;
            }

            memcpy(prev, curr, k);
            count = 1;
        }
    }

    if (close(fd_in) < 0) die("close input");
    if (close(fd_out) < 0) die("close output");
}

void decompressFile(const char *infile, const char *outfile, int k) {
    int fd_in = open(infile, O_RDONLY);
    if (fd_in < 0) die("open input");

    int fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_out < 0) die("open output");

    unsigned char count;
    unsigned char pattern[256];

    while (1) {
        ssize_t r = read(fd_in, &count, 1);
        if (r == 0) break; // EOF
        if (r < 0) die("read count");

        ssize_t p = read(fd_in, pattern, k);
        if (p < 0) die("read pattern");
        if (p == 0) break; // file ended unexpectedly

        for (int i = 0; i < count; i++) {
            if (write(fd_out, pattern, p) != p)
                die("write decompress");
        }
    }

    if (close(fd_in) < 0) die("close input");
    if (close(fd_out) < 0) die("close output");
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        write(STDERR_FILENO, "Usage: rle <input file> <output file> <compression length> <mode>\n", 67);
        exit(-1);
    }

    const char *input = argv[1];
    const char *output = argv[2];
    int k = atoi(argv[3]);
    int mode = atoi(argv[4]);

    if (k < 1) {
        write(STDERR_FILENO, "Error: compression length must be >= 1\n", 39);
        exit(-1);
    }

    if (mode != 0 && mode != 1) {
        write(STDERR_FILENO, "Error: mode must be 0 (compress) or 1 (decompress)\n", 52);
        exit(-1);
    }

    if (mode == 0)
        compressFile(input, output, k);
    else
        decompressFile(input, output, k);

    return 0;
}

