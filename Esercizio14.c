/*
 * Conversione in Network Order:
 * Implementa una funzione personalizzata che converte un numero a 16 bit
 * dal formato host al formato network (big-endian) senza usare htons.
 * Usa l’aritmetica dei puntatori e gli operatori di shift.
 *
 * Obiettivo:
 * Comprendere la rappresentazione BigEndian e le conversioni necessarie
 * per la comunicazione in rete.
 */

#include<stdio.h>
#include<stdlib.h>

/*
 * Funzione per invertire i byte di un short int (16 bit)
 */
short flip16(short num) {
    unsigned char *p = (unsigned char*) &num;
    // p[0] è il byte meno significativo, p[1] il più significativo
    // Ricostruisce il numero con i byte invertiti
    return (p[1]) | (p[0] << 8);
}

/*
 * Funzione per invertire i byte di un int (32 bit)
 */
int flip32(int num) {
    unsigned char *p = (unsigned char*) &num;
    // Ricostruisce il numero invertendo tutti i 4 byte
    return (p[3]) | (p[2] << 8) | (p[1] << 16) | (p[0] << 24);
}

void checkEndian() {
    unsigned int x = 1;        // valore 1
    char *c = (char*)&x;       // punta al primo byte di x

    if (*c == 1) {
        printf("Il sistema e' Little Endian\n"); // La maggior parte dei sistemi moderni utilizza l'endianness little-endian, tipico di architetture x86, x86-64 e ARM, mentre il big-endian è meno comune.
    } else {
        printf("Il sistema e' Big Endian\n");
    }
}


int main() {
    short int numero16 = 15;    // 16 bit, in binario è 00001111 comodo per flippare e capire.
    int numero32 = 305419896;   // 0x12345678 in esadecimale, esempio 32 bit

    checkEndian();

    // Flip 16 bit
    short flipped16 = flip16(numero16);
    printf("Originale 16 bit: %d, Flippato: %d\n", numero16, flipped16);

    // Flip 32 bit
    int flipped32 = flip32(numero32);
    printf("Originale 32 bit: %d, Flippato: %d\n", numero32, flipped32);

    return 0;
}
//Endian: ordine in cui i BYTE sono salvati in memoria, little prima quelli più piccoli, big prima quelli grandi.
