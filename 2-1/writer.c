#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "wb");
    
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return 1;
    }
    
    int number;
    
    // Запрашиваем у пользователя ввод чисел
    printf("Введите числа типа int (введите 0 для завершения):\n");
    while (1) {
        scanf("%d", &number);
        
        // Проверяем, был ли введен 0
        if (number == 0) {
            break;
        }
        
        // Записываем число в файл
        fwrite(&number, sizeof(int), 1, file);
    }
    
    fclose(file);
    
    printf("Числа были успешно записаны в файл '%s'\n", argv[1]);
    
    return 0;
}
