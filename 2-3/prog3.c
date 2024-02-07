#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <имя файла> <n>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int n = atoi(argv[2]);

    FILE *source = fopen(filename, "r");
    if (!source) {
        perror("Ошибка при открытии исходного файла");
        return 2;
    }

    char tempFilename[] = "tempfile.tmp";
    FILE *temp = fopen(tempFilename, "w");
    if (!temp) {
        fclose(source);
        perror("Ошибка при создании временного файла");
        return 3;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, source)) != -1) {
        if (strlen(line) != n + 1) { // +1 для учета символа новой строки
            fputs(line, temp);
        }
    }

    free(line);
    fclose(source);
    fclose(temp);

    remove(filename);
    rename(tempFilename, filename);

    printf("Операция успешно выполнена.\n");

    return 0;
}
