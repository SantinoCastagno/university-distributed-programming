#ifndef DICTIONARY_H_
#define DICTIONARY_H_

/* Estructura para un nodo de la tabla hash */
typedef struct Nodo {
    char* clave;
    char* valor;
    struct Nodo* siguiente;
} Nodo;

// Estructura para la tabla hash (Diccionario)
typedef struct {
    Nodo** tabla;
    int tamano;
} Diccionario;

// Funciones para el diccionario
Diccionario* crear_diccionario(int tamano);
void insertar(Diccionario* dic, char* clave, char* valor);
char* obtener(Diccionario* dic, char* clave);
void liberar_diccionario(Diccionario* dic);


#endif // DICTIONARY_H_
