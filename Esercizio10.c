// Operazioni Bitwise: Estrazione di un Bit
// Scrivi una funzione che, dato un char e una posizione di bit, restituisce il valore
// del bit (0 o 1) usando l’operatore bitwise & e lo shift.
// Obiettivo: Praticare l’uso degli operatori bitwise per manipolare singoli bit.

#include<stdio.h>
#include<stdlib.h>

void main(){

    char carattere = 'a'; //in binario 01100001
    
    for(int posizione = 7; posizione >= 0 ; posizione--){
        printf("%d", (carattere>>posizione)&1);
    }
}

//Cose che ho capito:
//come funziona (carattere>>posizione)&1:
//devo stampare ogni bit nell'ordine corretto, ovvero dal più significativo (che alla fine finisce davanti) al meno significativo
//quindi parto dal 7 bit, lo porto con lo shift >> nella posizione zero, faccio l'and bit a bit (&) con 1, in modo che i primi 7 zeri rimangano 0,
// e viene nella pratica valutato l'ultimo, che è o 0 (e ritorna 0) o 1 (e ritorna 1).
// quel valore viene quindi stampato, si passa al 6° valore e cosi via.
