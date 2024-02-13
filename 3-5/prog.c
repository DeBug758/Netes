#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void reverse_string(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        // Не реверсируем символ новой строки, если он есть
        if(str[i] != '\n' && str[n-i] != '\n') {
            char temp = str[i];
            str[i] = str[n-i-1];
            str[n-i-1] = temp;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd1[2], fd2[2]; // Каналы для коммуникации
    pid_t pid;
    FILE *file;

    // Создаем каналы
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Родительский процесс
        close(fd1[0]); // Закрываем неиспользуемый конец чтения
        close(fd2[1]); // Закрываем неиспользуемый конец записи

        file = fopen(argv[1], "r+");
        if (!file) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, file)) {
            write(fd1[1], buffer, strlen(buffer) + 1); // Пишем в канал

            // Читаем ответ
            read(fd2[0], buffer, BUFFER_SIZE);
            fseek(file, -strlen(buffer), SEEK_CUR); // Возвращаемся назад, чтобы перезаписать строку
            fputs(buffer, file);
        }

        close(fd1[1]); // Закрываем канал после использования
        close(fd2[0]);
        fclose(file);
    } else { // Дочерний процесс
        close(fd1[1]); // Закрываем неиспользуемый конец записи
        close(fd2[0]); // Закрываем неиспользуемый конец чтения

        char buffer[BUFFER_SIZE];
        while (read(fd1[0], buffer, BUFFER_SIZE) > 0) {
            reverse_string(buffer); // Реверсируем строку
            write(fd2[1], buffer, strlen(buffer) + 1); // Отправляем обратно
        }

        close(fd1[0]); // Закрываем канал после использования
        close(fd2[1]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
