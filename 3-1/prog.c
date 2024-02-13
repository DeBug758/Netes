#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid_son, pid_grandson1, pid_grandson2;

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
            printf("First grandson is doing something\n");
            sleep(1); // Имитация работы
            printf("First grandson finished\n");
            exit(0);
        }

        pid_grandson2 = fork();
        if (pid_grandson2 == 0) {
            // Это второй процесс-внук
            printf("Second grandson is doing something\n");
            sleep(1); // Имитация работы
            printf("Second grandson finished\n");
            exit(0);
        }

        // Процесс-сын ожидает завершения обоих процессов-внуков
        waitpid(pid_grandson1, NULL, 0);
        waitpid(pid_grandson2, NULL, 0);
        printf("Son process finished\n");
        exit(0);
    } else {
        // Это процесс-отец
        // Отец ожидает завершения процесса-сына
        waitpid(pid_son, NULL, 0);
        printf("Father process finished\n");
    }

    return EXIT_SUCCESS;
}
