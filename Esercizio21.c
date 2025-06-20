/*
 * 21. Ricezione in blocchi e simulazione congestione
 * - Leggi i dati ricevuti in blocchi da 1400 byte.
 * - Inserisci una pausa di 2 secondi (sleep(2)) tra le letture
 *   per osservare il comportamento del server e della coda di ricezione TCP (RX Queue).
 * - Obiettivi:
 *     * osservare la frammentazione TCP,
 *     * comprendere il controllo di flusso,
 *     * monitorare lo stato con netstat.
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main() {
    //AGGIUNTE//
    char* request = "GET /\r\n";
    char response[1000000];
    int x = 1;
    //FINEAGGIUNTE//
    int s;
    struct sockaddr_in server;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Creazione socket fallita");
        return 1;
    }
    printf("Socket creato: %d\n", s);
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr.s_addr = inet_addr("216.58.213.4");
    if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Connection fallita");
        close(s);
        return 1;
    }
    printf("Connessione riuscita a 216.58.213.4:80, google.com\n");


    printf("\n\n\n\n\nSEPARATORE DELLE PARTI\n\n\n\n\n");

    
    char * p = (char*)&response;
    write(s,request,strlen(request));
    int finale = x;
    while(x){
        x = read(s, p, 1400);
        printf("%s", p);
        printf("\n%d bytes\n", x);
        p = p + x;
        finale += x; 
        sleep(2);
    }
    *p = 0;
    printf("Tot bytes = %d\n", finale);
   // printf("RISPOSTA TOTALE = %s\n", response);
    return 0;
}

//USARE SU CMD: netstat -anp | grep :80
