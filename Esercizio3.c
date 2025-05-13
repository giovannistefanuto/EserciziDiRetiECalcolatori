//Aritmetica dei Puntatori:
  //Crea un programma che dichiara un array di short int e utilizza la notazione aritmetica dei puntatori per assegnare valori agli elementi. 
  //Verifica che la somma del puntatore incrementa l’indirizzo di sizeof(short int) byte ad ogni incremento.
  //Obiettivo: Consolidare la comprensione dell’aritmetica dei puntatori.

  #include<stdio.h>
  #include<stdlib.h>

  int main(){
    short int array[4]={1,2,3,4}; //2 byte per 4 elementi (verificabile con sizeof)
    printf("Dimensione arrary: %d\n", sizeof(array));
    short int *p=&array[0];
    for(int i=0; i<4; i++){
        *(p+i) = i + 19; //riempimento a caso da 19 a 22
        printf("indirizzo:%d\n", (p+i)); //l'indirizzo a cui il puntatore punta aumenta effettivamente di 2 byte
        printf("array posizione %d e' %d\n", i, array[i]);
    }
  }