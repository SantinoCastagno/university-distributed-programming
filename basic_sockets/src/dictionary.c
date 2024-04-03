#include "dictionary.h"
#include <stdlib.h>
#include <string.h>

// Función para crear un nuevo nodo
Nodo* nuevo_nodo(char* clave, char* valor) {
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo->clave = strdup(clave);
    nodo->valor = valor;
    nodo->siguiente = NULL;
    return nodo;
}
Diccionario* crear_diccionario(int tamano) {
    Diccionario* dic = malloc(sizeof(Diccionario));
    dic->tabla = malloc(tamano * sizeof(Nodo*));
    dic->tamano = tamano;
    for (int i = 0; i < tamano; i++) {
        dic->tabla[i] = NULL;
    }
    return dic;
}
// Función de hash
unsigned int hash(char* clave, int tamano) {
    unsigned int hash = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        hash = hash * 31 + clave[i];
    }
    return hash % tamano;
}

void insertar(Diccionario* dic, char* clave, char* valor) {
    unsigned int indice = hash(clave, dic->tamano);
    Nodo* nodo_actual = dic->tabla[indice];
    Nodo* nodo_nuevo = nuevo_nodo(clave, valor);
    if (nodo_actual == NULL) {
        dic->tabla[indice] = nodo_nuevo;
    } else {
        while (nodo_actual->siguiente != NULL) {
            nodo_actual = nodo_actual->siguiente;
        }
        nodo_actual->siguiente = nodo_nuevo;
    }
}

char* obtener(Diccionario* dic, char* clave) {
    unsigned int indice = hash(clave, dic->tamano);
    Nodo* nodo_actual = dic->tabla[indice];
    while (nodo_actual != NULL) {
        if (strcmp(nodo_actual->clave, clave) == 0) {
            return nodo_actual->valor;
        }
        nodo_actual = nodo_actual->siguiente;
    }
    return ""; // Clave no encontrada
}

void liberar_diccionario(Diccionario* dic) {
    for (int i = 0; i < dic->tamano; i++) {
        Nodo* actual = dic->tabla[i];
        while (actual != NULL) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            free(temp->clave);
            free(temp);
        }
    }
    free(dic->tabla);
    free(dic);
}
