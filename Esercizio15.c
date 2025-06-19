/*
 * Creazione di un Client TCP
 * Implementa un programma client TCP che:
 *     - crea un socket,
 *     - imposta l’indirizzo IP e la porta del server,
 *     - si connette al server,
 *     - stampa eventuali errori.
 * • Obiettivi principali:
 *     - uso della funzione socket(),
 *     - strutture sockaddr_in,
 *     - conversione della porta con htons(),
 *     - gestione degli errori con perror().
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include <unistd.h>  
    
int main() {
    int s;
    struct sockaddr_in server;
    
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Creazione socket fallita");
        return 1;
    }
    printf("Socket creato: %d\n", s);

    //Riempimento della struttura sockaddr_in
    server.sin_family = AF_INET;
    server.sin_port = htons(80); //80 in BigEndian //80 La porta 80 è la porta standard del protocollo HTTP
    server.sin_addr.s_addr = inet_addr("96.7.128.198"); // IP di example.com
    if(-1 == connect(s, (struct sockaddr *) &server, sizeof(struct sockaddr_in))){
        perror("Connection fallita");
        return 1;
    }
    printf("Connessione riuscita a 96.7.128.198:80\n");

    close(s);  // buona pratica
    
    return 0;
}
