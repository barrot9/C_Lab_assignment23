#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER 28

typedef struct Node {
    char *filename;
    int count;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} HashTableEntry;

HashTableEntry hashTable[MAX_NUMBER + 1];

void initializeHashTable() {
    for (int i = 0; i <= MAX_NUMBER; ++i) {
        hashTable[i].head = NULL;
    }
}

Node* createNode(const char *filename) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->filename = strdup(filename);
    newNode->count = 1;
    newNode->next = NULL;
    return newNode;
}

void insertOrUpdate(const char *filename, int number) {
    Node *current = hashTable[number].head;
    Node *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->filename, filename) == 0) {
            current->count++;
            return;
        }
        previous = current;
        current = current->next;
    }

    Node *newNode = createNode(filename);
    if (previous == NULL) {
        hashTable[number].head = newNode;
    } else {
        previous->next = newNode;
    }
}

void processFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int number;
    while (fscanf(file, "%d", &number) == 1) {
        if (number >= 0 && number <= MAX_NUMBER) {
            insertOrUpdate(filename, number);
        }
    }

    fclose(file);
}

void printSummary() {
    for (int i = 0; i <= MAX_NUMBER; ++i) {
        Node *current = hashTable[i].head;
        if (current != NULL) {
            printf("%d appears in", i);
            while (current != NULL) {
                printf(" file %s - %d time%s", current->filename, current->count, current->count > 1 ? "s" : "");
                current = current->next;
                if (current != NULL) {
                    printf(",");
                }
            }
            printf("\n");
        }
    }
}

void freeHashTable() {
    for (int i = 0; i <= MAX_NUMBER; ++i) {
        Node *current = hashTable[i].head;
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp->filename);
            free(temp);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    initializeHashTable();

    for (int i = 1; i < argc; ++i) {
        processFile(argv[i]);
    }

    printSummary();
    freeHashTable();

    return EXIT_SUCCESS;
}
