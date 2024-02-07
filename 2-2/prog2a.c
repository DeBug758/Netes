#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 120

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <character>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char tempFilename[] = "tempfileXXXXXX";
    int fd = mkstemp(tempFilename);
    if (fd == -1) {
        perror("Error creating temporary file");
        fclose(file);
        return 1;
    }
    FILE *tempFile = fdopen(fd, "w");

    char line[MAX_LINE_LENGTH];
    char *characterToFind = argv[2];

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (strchr(line, characterToFind[0]) != NULL) { // Измените условие на " == NULL", чтобы удалить строки, не содержащие символ
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Заменяем оригинальный файл временным файлом
    remove(argv[1]);
    rename(tempFilename, argv[1]);

    printf("Finished processing.\n");

    return 0;
}
