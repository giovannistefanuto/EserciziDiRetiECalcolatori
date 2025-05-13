//Allocazione Dinamica di un Array: Utilizzando malloc e free,
//scrivi un programma che alloca dinamicamente un array di interi della dimensione specificata dall’utente, inizializza gli elementi
//e poi libera la memoria. Obiettivo: Gestire la memoria dinamica (heap).
#include<stdio.h>
#include<stdlib.h>

int main(){
    int decisione;
    printf("L'array parte da 0.\nQuanto grande vuoi l'array: ");
    scanf("%d", &decisione);
    //allocazione dinamica su un puntatore
    int* p = malloc (sizeof(int)*decisione);
    for(int i=0; i<decisione; i++)
    p[i] = i; //giusto per capire
 // p[1000] = 5; //COMPORTAMENTO IMPREVEDIBILE, potrebbe crashare, potrebbe andare tutto bene, di sicuro non da SEMPRE OutOffBounds
    for(int i=0; i<decisione; i++)
    printf("il valore dell'array in posizione i = %d, e' %d\n",i,p[i]);
    free(p); //sempre necessario liberare la memoria, altrimenti c'è dangling pointer (BRUTTO BRUTTO)
    return 0;
}
