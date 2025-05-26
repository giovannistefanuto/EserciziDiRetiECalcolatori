// Manipolazione di Stringhe (Array di Caratteri): Scrivi una funzione che copia una stringa in un’altra, facendo uso degli array di
// caratteri e della terminazione con il carattere nullo. 
// Non utilizzare funzioni standard come strcpy. Obiettivo: Rivedere la gestione delle stringhe in C.
#include <stdio.h>
#include <stdlib.h>

// Funzione che copia una stringa (parolina) in un'altra (parolone)
void copia_a_in_b(char *destinazione, char *sorgente) {
    int i = 0;
    while (sorgente[i] != '\0') {   // finché non si incontra il terminatore
        destinazione[i] = sorgente[i]; // copia ogni carattere
        i++;
    }
    destinazione[i] = '\0'; // importante! Aggiungi il terminatore alla fine
}

int main() {
    char parola1[6] = "ciao"; // spazio per 5 caratteri + '\0'
    char parola2[6] = "casa"; // stessa dimensione

    copia_a_in_b(parola1, parola2); // copia parola2 in parola1

    printf("la prima parola e' %s\n", parola1);
    printf("la seconda parola e' %s\n", parola2);

    return 0;
}

//Da questo esercizio ho capito che:
// Il terminatore di una stringa è il carattere '\0' ovvero lo zero binario.
// Ricordarsi che quando definisci una stringa char nome[numero_caratteri]="parola con un carattere in meno di numero_caratteri" il nome
// è un puntatore alla prima parola, quindi nelle funzioni esterne passo direttamente "nome"
// Aggiungere sempre il terminatore alla fine della stringa, altrimenti c'è un comportamento imprevedibile.

