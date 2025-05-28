#include <stdio.h>
#include <stdint.h>  // Per uintptr_t

// Definizione della struttura per osservare il padding
struct qualsiasi {
    short int a;     // 2 byte
    char carattere;  // 1 byte
    short int b;     // 2 byte, richiede allineamento a 2 byte
};

int main() {
    // Stampa della dimensione dei singoli tipi
    printf("Dimensione di short int: %zu byte\n", sizeof(short int));
    printf("Dimensione di char: %zu byte\n", sizeof(char));

    // Stampa della dimensione attesa "logica" (senza padding)
    printf("Somma delle dimensioni dei campi (senza padding): %zu byte\n", sizeof(short int) * 2 + sizeof(char));

    // Stampa della dimensione reale della struct (con padding)
    printf("Dimensione reale della struct qualsiasi (con padding): %zu byte\n", sizeof(struct qualsiasi));

    // Creazione di una variabile della struct
    struct qualsiasi Gianni = {10, 'b', 20};

    // Stampa degli indirizzi dei singoli campi
    printf("Indirizzo di Gianni.a:        %p\n", (void*)&Gianni.a);
    printf("Indirizzo di Gianni.carattere: %p\n", (void*)&Gianni.carattere);
    printf("Indirizzo di Gianni.b:        %p\n", (void*)&Gianni.b);

    // Calcolo della differenza in byte tra i campi
    printf("Differenza in byte tra &Gianni.b e &Gianni.a: %ld byte\n",(intptr_t)((char*)&Gianni.b - (char*)&Gianni.a));

    printf("Differenza in byte tra &Gianni.carattere e &Gianni.a: %ld byte\n",(intptr_t)((char*)&Gianni.carattere - (char*)&Gianni.a));
    return 0;
}

// Cosa ho imparato:
// - In una struct i campi non sono puntatori, ma variabili che occupano memoria contigua.
// - &Gianni.a significa "indirizzo di memoria del campo a", cioè un puntatore a quel campo.
// - La differenza tra due puntatori di tipo T* è la distanza in numero di elementi di tipo T, non in byte.
// - Per calcolare la differenza in byte tra indirizzi, devo prima convertire i puntatori a (char*), perché char occupa 1 byte.
// - Il padding è spazio aggiunto dal compilatore per allineare i dati in memoria, migliorando la velocità di accesso.
// - Nel nostro esempio, la struct occupa più byte della somma dei singoli campi proprio a causa del padding.
// - Usare sizeof mi aiuta a capire quanto spazio occupa realmente una struct con padding incluso.
//
// - Perché (intptr_t)?
// Quando sottraggo due puntatori (es. (char*)p1 - (char*)p2), ottengo una differenza in byte.
// Per stampare questo valore con %ld, serve un cast a un tipo intero compatibile.
// intptr_t (da <stdint.h>) è un tipo intero grande quanto un puntatore, sicuro e portabile su tutte le architetture.

