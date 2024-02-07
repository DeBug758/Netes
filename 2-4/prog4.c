#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определение структуры для элемента списка
typedef struct WordNode {
    char word[101]; // Предполагаем, что одно слово не длиннее 100 символов
    struct WordNode* next;
} WordNode;

// Функция для добавления слова в начало списка
WordNode* addWord(WordNode* head, const char* word) {
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    if (!newNode) {
        perror("Failed to allocate memory for a new node");
        exit(1);
    }
    strcpy(newNode->word, word);
    newNode->next = head;
    return newNode;
}

// Функция для вывода списка в обратном порядке
void printListReverse(WordNode* head) {
    if (head == NULL) return;
    printListReverse(head->next);
    printf("%s\n", head->word);
}

// Функция для вывода списка
void printList(WordNode* head) {
    while (head != NULL) {
        printf("%s\n", head->word);
        head = head->next;
    }
    printf("\n");
}

// Функция для освобождения памяти списка
void freeList(WordNode* head) {
    WordNode* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// Функция для чтения слов со стандартного ввода
WordNode* createListFromStdin() {
    char word[101];
    WordNode* head = NULL;
    while (scanf("%100s", word) == 1) {
        if (word[0] == '\n' || word[0] == EOF) break;
        head = addWord(head, word);
    }
    return head;
}

// Функция для чтения слов из файла
WordNode* createListFromFile(char* filename, int lineNumber) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }
    char line[1024]; // Предполагаем, что длина строки не превышает 1023 символов
    WordNode* head = NULL;
    int currentLine = 1;
    while (fgets(line, sizeof(line), file)) {
        if (currentLine == lineNumber) {
            char* word = strtok(line, " \n");
            while (word != NULL) {
                head = addWord(head, word);
                word = strtok(NULL, " \n");
            }
            break;
        }
        currentLine++;
    }
    fclose(file);
    if (currentLine < lineNumber) {
        return NULL; // Указанной строки в файле нет
    }
    return head;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <lineNumber>\n", argv[0]);
        WordNode* list = createListFromStdin();
        if (list == NULL) {
        printf("Line in stdin not found.\n");
        return 1;
    }
    printList(list);
    freeList(list);
        return 0;
    }
    int lineNumber = atoi(argv[2]);
    WordNode* list = createListFromFile(argv[1], lineNumber);
    if (list == NULL) {
        printf("No line %d in file or file not found.\n", lineNumber);
        return 1;
    }
    printList(list);
    freeList(list);
    return 0;
}
