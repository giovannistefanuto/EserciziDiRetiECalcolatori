// Toggle di un Bit:
// Implementa una funzione int toggle_bit(int val, unsigned char pos) 
// che inverte il bit alla posizione specificata usando l’operatore XOR (^).
// Obiettivo: Applicare le operazioni bit a bit per manipolare informazioni.

#include<stdio.h>
#include<stdlib.h>

int toggle_bit(int val, unsigned char pos){
    return val ^ (1 << (7-pos));
}


void main(){

    char carattere = 'a'; //in binario 01100001
    //voglio che sia 'e' ovvero 01100101, quindi cambiare bit in posizione = 5
    unsigned char pos = 5;

    printf("carattere: %c\n", carattere);
    for(int posizione = 7; posizione >= 0 ; posizione--){
        printf("%d", (carattere>>posizione)&1);
    }
    printf("\n");
    carattere = toggle_bit(carattere, pos);

    printf("carattere: %c", carattere); printf("\n");
    for(int posizione = 7; posizione >= 0 ; posizione--){
        printf("%d", (carattere>>posizione)&1);
    }
}

// L'operatore XOR (^) confronta due bit e restituisce 1 se sono diversi, 0 se sono uguali.
// Esempi:
// 0 ^ 0 = 0
// 1 ^ 0 = 1
// 0 ^ 1 = 1
// 1 ^ 1 = 0
//
// In pratica, XOR "toglie" il bit se era 1, o lo "accende" se era 0.
// Per questo è utile per "inveritre" (toggle) un singolo bit senza modificare gli altri.
//
// Cosa ho imparato:
// Creo una maschera con un 1 spostato dalla fine (00000001) alla posizione desiderata usando (1 << (7 - pos)). Nel nostro caso si sposta di 2 posizioni, quindi 0000100
// Poi applica l'operatore XOR (^) tra `val` e questa maschera.
// L'XOR con 1 inverte il bit corrispondente, mentre gli altri bit rimangono invariati.
// L'uso di (7 - pos) serve a considerare la posizione del bit partendo dal bit più significativo (a sinistra).

