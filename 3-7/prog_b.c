#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLEN 7

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd1[2], fd2[2];
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        close(fd1[1]); // Close write end of fd1
        close(fd2[0]); // Close read end of fd2

        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        char notify;
        while (read(fd1[0], &notify, 1) > 0) {
            char line[MAXLEN];
            if (fgets(line, MAXLEN, file) != NULL) {
                printf("%s", line);
                write(fd2[1], "c", 1); // Notify parent
            } else {
                break; // End of file or error
            }
        }

        fclose(file);
        close(fd1[0]);
        close(fd2[1]);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        close(fd1[0]); // Close read end of fd1
        close(fd2[1]); // Close write end of fd2

        int lineno = 0;
        char notify = 'p';
        while (write(fd1[1], &notify, 1) > 0) {
            if (read(fd2[0], &notify, 1) > 0) {
                printf("Line %d\n", ++lineno);
            } else {
                break; // Child process finished
            }
        }

        close(fd1[1]);
        close(fd2[0]);
        wait(NULL); // Wait for child to prevent zombie process
    }

    return EXIT_SUCCESS;
}

