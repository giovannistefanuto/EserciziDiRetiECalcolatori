#include <stdio.h>
#include <stdlib.h>

int main() {
    // Definizione di un array di interi
    int array[4] = {1, 2, 3, 4};

    // Stampa degli elementi dell'array utilizzando la notazione classica con le parentesi
    printf("Accesso tramite array[i]:\n");
    for (int i = 0; i < 4; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }

    // Definizione di un puntatore che punta al primo elemento dell'array
    int *p = array;  // equivalente a &array[0]

    // Stampa degli elementi dell'array utilizzando il puntatore
    printf("\nAccesso tramite puntatore *(p+i):\n");
    for (int i = 0; i < 4; i++) {
        printf("*(p + %d) = %d\n", i, *(p + i));
    }

    // Dimostrazione dell'equivalenza tra array[i] e *(array + i)
    printf("\nDimostrazione: array[1] = %d, *(array + 1) = %d\n", array[1], *(array + 1));

    return 0;
}

