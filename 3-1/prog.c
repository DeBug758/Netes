#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid_son, pid_grandson1, pid_grandson2;
	int fd[2];
	if(pipe(fd) == -1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
    pid_son = fork();
    if (pid_son == -1) {
        // Ошибка при создании процесса-сына
        perror("fork");
        return EXIT_FAILURE;
    } else if (pid_son == 0) {
        // Это процесс-сын
        pid_grandson1 = fork();
        if (pid_grandson1 == 0) {
            // Это первый процесс-внук
			close(fd[0]);
            printf("First grandson is doing something\n");
            sleep(1); // Имитация работы
            printf("First grandson finished\n");
			write(fd[1], "200", 3);
			close(fd[1]);
            exit(0);
        }

        pid_grandson2 = fork();
        if (pid_grandson2 == 0) {
            // Это второй процесс-внук
			close(fd[0]);
            printf("Second grandson is doing something\n");
            sleep(1); // Имитация работы
            printf("Second grandson finished\n");
			write(fd[1], "200", 3);
			close(fd[1]);
            exit(0);
        }
        printf("Son process finished\n");
        exit(0);
    } else {
		close(fd[1]);
        // Это процесс-отец
        // Отец ожидает завершения процессов-внуков
		char buffer[5];
		read(fd[0], buffer, sizeof(buffer));
		if(buffer[0] == '2' && buffer[1] == '0' && buffer[2] == '0'){
			read(fd[0], buffer, sizeof(buffer));
		}
		else{
			perror("pipe");
			return EXIT_FAILURE;
		}
        printf("Father process finished\n");
		close(fd[0]);
    }

    return EXIT_SUCCESS;
}
