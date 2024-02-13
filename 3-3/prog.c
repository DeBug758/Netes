#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source file> <destination file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    FILE *src = fopen(argv[1], "r");
    if (src == NULL) {
        perror("fopen source file");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Процесс-сын
        close(pipefd[1]); // Close write end of pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect read end of pipe to stdin
        close(pipefd[0]); // Close read end of pipe, stdin is now pipe

        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dest_fd == -1) {
            perror("open destination file");
            exit(EXIT_FAILURE);
        }
        dup2(dest_fd, STDOUT_FILENO); // Redirect stdout to destination file
        close(dest_fd); // Close file descriptor, stdout is now destination file

        execlp("sort", "sort", (char *)NULL);
        // If execlp returns, it's an error
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Процесс-отец
        close(pipefd[0]); // Close read end of pipe
        FILE *pipe_stream = fdopen(pipefd[1], "w");
        if (pipe_stream == NULL) {
            perror("fdopen");
            exit(EXIT_FAILURE);
        }

        char *line = NULL;
        size_t len = 0;
        ssize_t nread;
        int line_number = 0;
        while ((nread = getline(&line, &len, src)) != -1) {
            line_number++;
            if (line_number % 2 == 0) {
                fputs(line, pipe_stream);
            }
        }
        free(line);
        fclose(src);
        fclose(pipe_stream); // Close write end of pipe, triggers EOF for sort

        wait(NULL); // Ждем окончания процесса сына
    }

    return EXIT_SUCCESS;
}

