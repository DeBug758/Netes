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

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Процесс-сын
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

		FILE *src = fopen(argv[1], "r");
		if(src == NULL){
			perror("fopen source file");
			return EXIT_FAILURE;
		}

        char line[1000];
        size_t bytesRead;
        int cnt = 1;
        while (fgets(line, sizeof(line), src) != NULL) {
            if(cnt % 2 == 0){
                printf("%s", line); // Output for demonstration
            }
            cnt++;
        }
        fclose(src);
        close(fd[1]);
        puts("Son is done");
        exit(0);

    } else {
        // Процесс-отец
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dest_fd == -1) {
            perror("open destination file");
            exit(EXIT_FAILURE);
        }
        dup2(dest_fd, STDOUT_FILENO);
        close(dest_fd);
        execlp("sort", "sort", (char *)NULL);
        // If execlp returns, it's an error
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

