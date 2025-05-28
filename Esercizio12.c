/*
 * Shift Operators per Moltiplicazione/Divisione:
 * Scrivi una funzione che, dato un numero intero,
 * ne calcoli il doppio e la metà usando gli operatori << e >>.
 * 
 * Obiettivo:
 * Comprendere l’equivalenza degli shift con la moltiplicazione
 * e la divisione per potenze di 2.
 */

#include<stdio.h>
#include<stdlib.h>

void doppio(int numero){
    int doppio = numero<<1;
    if(doppio>numero){
        printf("il doppio di %d e' %d\n", numero, numero<<1);
        printf("la meta' di %d e' %d", numero, numero>>1);
    }
    else
        printf("Errore di Overflow");
}

int main(){
    int a = 20;
    doppio(a);
}