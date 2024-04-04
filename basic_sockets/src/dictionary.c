#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Función para crear un nuevo node
Node* create_node(char* key, char* value) {
    Node* node = malloc(sizeof(Node));
    node->key = strdup(key);
    node->value = value;
    node->next = NULL;
    return node;
}
Dictionary* create_dictionary(int size) {
    Dictionary* dic = malloc(sizeof(Dictionary));
    dic->table = malloc(size * sizeof(Node*));
    dic->size = size;
    for (int i = 0; i < size; i++) {
        dic->table[i] = NULL;
    }
    return dic;
}
// Función de hash
unsigned int hash(char* key, int size) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = hash * 31 + key[i];
    }
    return hash % size;
}

void insert(Dictionary* dic, char* key, char* value) {
    unsigned int index = hash(key, dic->size);
    Node* actual_node = dic->table[index];
    Node* new_node = create_node(key, value);
    if (actual_node == NULL) {
        dic->table[index] = new_node;
    } else {
        while (actual_node->next != NULL) {
            actual_node = actual_node->next;
        }
        actual_node->next = new_node;
    }
}

char* get(Dictionary* dic, char* key) {
    unsigned int index = hash(key, dic->size);
    Node* actual_node = dic->table[index];
    while (actual_node != NULL) {
        if (strcmp(actual_node->key, key) == 0) {
            return actual_node->value;
        }
        actual_node = actual_node->next;
    }
    return ""; // key no encontrada
}

void free_dictionary(Dictionary* dic) {
    printf("DEBUG: ALERT FREE.\n");
    for (int i = 0; i < dic->size; i++) {
        Node* actual = dic->table[i];
        while (actual != NULL) {
            Node* temp = actual;
            actual = actual->next;
            free(temp->key);
            free(temp);
        }
    }
    free(dic->table);
    free(dic);
}
