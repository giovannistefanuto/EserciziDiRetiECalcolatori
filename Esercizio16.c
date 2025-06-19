/*
 * Conversione manuale di byte order (htons custom)
 * • Scrivi una funzione che converte un numero da host a network
 *   byte order senza usare htons().
 * • Obiettivi: manipolazione dei byte, endianness
 */

#include <stdio.h>

// Funzione che inverte i byte di un intero a 32 bit
unsigned int invertiByte(unsigned int x) {
    return ((x & 0xFF) << 24) |
           ((x & 0xFF00) << 8) |
           ((x & 0xFF0000) >> 8) |
           ((x & 0xFF000000) >> 24);
}

int main() {
    unsigned int x = 1;
    char *c = (char*)&x;

    if (*c == 1) { // se il primo byte è 1, è Little Endian
        printf("Little Endian\n");

        unsigned int numero = 0x12345678;
        unsigned int invertito = invertiByte(numero);

        printf("Originale: 0x%X\n", numero);
        printf("Invertito: 0x%X\n", invertito);
    } else {
        printf("Big Endian\n");
    }

    return 0;
}
