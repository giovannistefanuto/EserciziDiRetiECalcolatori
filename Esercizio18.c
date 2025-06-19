/*
 * (d) Stampa di indirizzo IP da puntatore
 * • Funzione che riceve un unsigned char* e stampa l’IP in notazione puntata.
 * • Obiettivi: manipolazione di byte, notazione umana degli IP.
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include <unistd.h>  
//COME ES 15.C
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
    server.sin_port = htons(80); 
    server.sin_addr.s_addr = inet_addr("96.7.128.198"); // IP di example.com
    char *p = (char*)&server.sin_addr.s_addr;
    printf("%u.%u.%u.%u\n", (unsigned char)p[0], (int)p[1], (unsigned char)p[2], (unsigned char)p[3]);

    
    return 0;
}