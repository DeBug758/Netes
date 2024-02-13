#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MAXLEN 7

// Глобальные переменные для синхронизации
volatile sig_atomic_t canPrint = 0;

// Обработчики сигналов
void handle_parent(int sig) {
    canPrint = 1;
}

void handle_child(int sig) {
    canPrint = 1;
}

void handle_alarm(int sig) {
    // Обработчик сигнала SIGALRM для завершения работы сына
    printf("Время работы сына истекло (3 секунды).\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя файла>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR1, handle_parent);
    signal(SIGUSR2, handle_child);

    pid_t pid = fork();
    if (pid == -1) {
        // Ошибка создания процесса
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Родительский процесс (отец)
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        char line[MAXLEN];
        int lineno = 0;
        while (fgets(line, MAXLEN, file) != NULL && lineno < MAXLEN) {
            while (!canPrint) ; // Ожидание сигнала от сына
            printf("Строка №%d\n", ++lineno);
            canPrint = 0;
            kill(pid, SIGUSR2); // Отправка сигнала сыну
        }

        fclose(file);
        kill(pid, SIGINT); // Завершение работы сына
        wait(NULL); // Ожидание завершения сына
    } else {
        // Дочерний процесс (сын)
        signal(SIGALRM, handle_alarm);
        alarm(3); // Установка таймера на 3 секунды

        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        char line[MAXLEN];
        while (fgets(line, MAXLEN, file) != NULL) {
            kill(getppid(), SIGUSR1); // Отправка сигнала отцу
            while (!canPrint) ; // Ожидание сигнала от отца
            printf("%s", line);
            canPrint = 0;
        }

        fclose(file);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
