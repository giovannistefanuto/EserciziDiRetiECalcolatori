//Definizione e Uso di Strutture: Definisci una struttura chiamata struct esempio che contenga caratteri e interi.
//Crea una variabile di questo tipo, assegna valori ai campi e stampa i valori. 
//Obiettivo: Applicare il concetto di strutture e l’ordine di allocazione dei campi.

#include<stdio.h>
#include<stdlib.h>

struct persona{
    int eta;
    char nome[50];
};

int main(){
    struct persona Gianni = {10, "Gianni"};
    printf("eta di Gianni: %d, nome di Gianni: %s", Gianni.eta, Gianni.nome);
}