/* Utilizzo di Puntatori di Base: Scrivi un semplice programma che dichiara tre variabili intere (x, y, z) e un puntatore ad intero.
Inizialmente, imposta il puntatore per puntare a ciascuna variabile in sequenza e stampa l’indirizzo memorizzato, utilizzando il formato 
esadecimale (%lx). Obiettivo: Comprendere la dichiarazione e l’uso dei puntatori. */

#include <stdio.h> // <--- Include la libreria standard di input/output
#include <stdlib.h> // <--- Include la libreria standard per le funzioni di allocazione della memoria

int main(){
    int a=10;
    int b=20;
    int c=30;
    int*ptr = &a;
    printf("a e' all'indirizzo: %d\n", ptr); //%d %d è decimale, %x è esadecimale, %o è ottale, %f è floating-point, %c è caratteri e %s per stringhe
    ptr = &b;
    printf("b e' all'indirizzo: %d\n", ptr);
    ptr = &c;
    printf("c e' all'indirizzo: %d\n", ptr);
    //Sono allocati in ordine dal più alto al più basso perchè Stack (main è una funzione) SCENDE
}
