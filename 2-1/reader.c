#include <stdio.h>

int main(int argc, char *argv[]) {
    // Открываем файл для чтения в бинарном режиме
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Не удалось открыть файл");
        return 1;
    }

    // Читаем данные из файла до тех пор, пока не достигнем конца файла
    while (!feof(file)) {
        // Читаем целое число из файла
        int number1;
        size_t bytesRead1 = fread(&number1, sizeof(int), 1, file);
        
        // Проверяем, достигнут ли конец файла после чтения целого числа
        if (bytesRead1 != 1)
            break;

        // Выводим число на экран
        printf("%d\n", number1);
    }

    // Закрываем файл
    fclose(file);

    return 0;
}
