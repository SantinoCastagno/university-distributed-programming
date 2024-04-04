#ifndef DICTIONARY_H_
#define DICTIONARY_H_

/* Estructura para un nodo de la tabla hash */
typedef struct Node {
    char* key;
    char* value;
    struct Node* next;
} Node;

// Estructura para la tabla hash (Diccionario)
typedef struct {
    Node** table;
    int size;
} Dictionary;

// Funciones para el diccionario
Dictionary* create_dictionary(int size);
void insert(Dictionary* dic, char* key, char* value);
char* get(Dictionary* dic, char* key);
void free_dictionary(Dictionary* dic);


#endif // DICTIONARY_H_
