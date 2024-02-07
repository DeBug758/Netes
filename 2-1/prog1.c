#include <stdio.h>
#include <stdlib.h>

void mergeFiles(const char *inputFile1, const char *inputFile2, const char *outputFile) {
    FILE *file1 = fopen(inputFile1, "rb");
    FILE *file2 = fopen(inputFile2, "rb");
    FILE *fileOut = fopen(outputFile, "wb");

    if (!file1 || !file2 || !fileOut) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

    int num1, num2;
    size_t read1, read2;

    // Читаем первые числа из обоих файлов
    read1 = fread(&num1, sizeof(int), 1, file1);
    read2 = fread(&num2, sizeof(int), 1, file2);

    // Пока есть данные в обоих файлах
    while (read1 && read2) {
        if (num1 < num2) {
            fwrite(&num1, sizeof(int), 1, fileOut);
            read1 = fread(&num1, sizeof(int), 1, file1);
        } else {
            fwrite(&num2, sizeof(int), 1, fileOut);
            read2 = fread(&num2, sizeof(int), 1, file2);
        }
    }

    // Если остались числа в первом файле
    while (read1) {
        fwrite(&num1, sizeof(int), 1, fileOut);
        read1 = fread(&num1, sizeof(int), 1, file1);
    }

    // Если остались числа во втором файле
    while (read2) {
        fwrite(&num2, sizeof(int), 1, fileOut);
        read2 = fread(&num2, sizeof(int), 1, file2);
    }

    fclose(file1);
    fclose(file2);
    fclose(fileOut);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <inputfile1> <inputfile2> <outputfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    mergeFiles(argv[1], argv[2], argv[3]);

    return EXIT_SUCCESS;
}

