//Scambio di Due Variabili con Puntatori:
//Scrivi una funzione void swap(int *a, int *b) che scambia i valori di due variabili intere mediante l’uso dei puntatori. 
//Testa la funzione nel main()
//Obiettivo: Approfondire l’uso dei puntatori per manipolare dati.
#include<stdio.h>
#include<stdlib.h>
void swap (int *a, int *b){
    int c = *a;
    *a = *b;
    *b = c;
}


int main(){
    int a = 10;
    int b = 20;
    printf("a:%d, b:%d\n", a,b);
    swap(&a,&b);
    printf("a:%d, b:%d\n", a,b);

    return 0;
}