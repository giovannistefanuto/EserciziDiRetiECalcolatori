
// Esercizio 6: Array di Puntatori
// Crea un array di puntatori a char per memorizzare una serie di stringhe.
// Inizializza e stampa ciascuna stringa.
// Obiettivo: Comprendere le relazioni tra array e puntatori, applicandolo
// alle stringhe.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
    char *p[2];
    char parola[5]="ciao"; //5 = 4 caratteri + terminatore
    char parola2[6] = "mondo";
    p[0] = parola;
    p[1] = parola2;
    printf("la prima parola e' %s", p[0]);
    printf("la seconda parola e' %s", p[1]);
    return 0;
}

//La chiave di questo esercizio è dire che per le stringhe (array di char) il nome della stringa "parola" è GIà UN PUNTATORE, quindi non serve fare p=&parola e mmeno fare 
// printf("la prima parola e' %s", *p[0]) perchè il puntatore p[0] punta già alla stringa parola.
// In questo caso, p[0] è un puntatore alla stringa "ciao" e p[1] è un puntatore alla stringa "mondo".
// Quindi, quando stampiamo p[0], stiamo stampando il contenuto della stringa "ciao".
//
// In questo caso, non è necessario dereferenziare il puntatore p[0] per stampare la stringa,
// perché il formato %s in printf accetta direttamente un puntatore a char (cioè una stringa).
