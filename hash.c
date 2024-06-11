#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER 28

/* Node structure to store filename and count of occurrences */
typedef struct Node {
    char *filename;
    int count;
    struct Node *next;
} Node;

/* HashTableEntry structure to store the head of the linked list for each number */
typedef struct {
    Node *head;
} HashTableEntry;

/* Hash table with an entry for each number from 0 to MAX_NUMBER */
HashTableEntry hashTable[MAX_NUMBER + 1];

/* Function to initialize the hash table */
void initializeHashTable() {
    for (int i = 0; i <= MAX_NUMBER; ++i) {
        hashTable[i].head = NULL;
    }
}

/* Function to create a new node with the given filename */
Node* createNode(const char *filename) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->filename = strdup(filename);
    newNode->count = 1;
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a new node or update the count of an existing node */
void insertOrUpdate(const char *filename, int number) {
    Node *current = hashTable[number].head;
    Node *previous = NULL;

    /* Traverse the linked list to find the filename or the end of the list */
    while (current != NULL) {
        if (strcmp(current->filename, filename) == 0) {
            current->count++;
            return;
        }
        previous = current;
        current = current->next;
    }

    /* Create a new node if the filename was not found */
    Node *newNode = createNode(filename);
    if (previous == NULL) {
        hashTable[number].head = newNode;
    } else {
        previous->next = newNode;
    }
}

/* Function to process a file and update the hash table */
void processFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int number;
    /* Read numbers from the file and update the hash table */
    while (fscanf(file, "%d", &number) == 1) {
        if (number >= 0 && number <= MAX_NUMBER) {
            insertOrUpdate(filename, number);
        }
    }

    fclose(file);
}

/* Function to print the summary of number occurrences in files */
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

/* Function to free the memory allocated for the hash table */
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

    /* Process each file provided as command line arguments */
    for (int i = 1; i < argc; ++i) {
        processFile(argv[i]);
    }

    printSummary();
    freeHashTable();

    return EXIT_SUCCESS;
}
