/*
 * Stampa di una Dump di Memoria:
 * Realizza un programma che, dato un array di byte,
 * stampi il contenuto in formato esadecimale,
 * simulando un dump della memoria.
 *
 * Obiettivo:
 * Lavorare direttamente con la memoria e la formattazione dell’output.
 */
#include<stdio.h>
#include<stdlib.h>

int main(){
    int array[5] = {0,1,2,3,4};
    char * p = (char*)&array;
    int contatore = 0;
    for(int i = 0; i < sizeof(array); i++){
        printf("%p\n",p++);
        contatore++;
    }
printf("Sono stati stampati %d indirizzi, ognuno corrispondente a 1 byte di memoria\n", contatore);
}

//pre incremento = prima incremento, poi uso = mettere ++ prima del nome
//post incremento = prima uso, poi incremento = mettere ++ dopo il nome
//Memoria è allocata a byte.
