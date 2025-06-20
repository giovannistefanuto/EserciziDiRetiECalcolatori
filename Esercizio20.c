/*
 * Esercizio 20 - Client HTTP completo: invio richiesta e ricezione risposta
 *
 * Questo programma implementa un semplice client TCP che:
 *  - Si connette a un server remoto (host e porta specificati)
 *  - Invia una richiesta HTTP/0.9 nella forma "GET /\r\n"
 *  - Riceve la risposta in modo incrementale tramite read() in ciclo
 *  - Utilizza un buffer preallocato per la lettura dei dati
 *  - Stampa la risposta ricevuta e il numero totale di byte letti
 *
 * Obiettivi didattici:
 *  - Utilizzo delle primitive di comunicazione write() e read()
 *  - Gestione dello stream TCP tramite un ciclo di lettura
 *  - Bufferizzazione efficiente dei dati ricevuti
 *  - Comprensione del protocollo HTTP/0.9 e delle sue differenze rispetto a versioni successive
 *
 * N.B.: HTTP/0.9 non prevede intestazioni né codici di stato: la risposta è solo contenuto (tipicamente HTML)
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
   //AGGIUNTE//

   /* STA COSA STAMPA OGNI TOT E SOVRASCRIVE, NON SCARICA TUTTA LA PAGINA.
    write(s,request,strlen(request));
    while(x){
        x = read(s, response, 999999);
        response[x] = 0;
        printf("%s", response);
        printf("\n%d bytes\n", x);
    }
*/

    //questo invece accumula il messaggio man mano dentro response, di pezzo in pezzo.
    write(s, request, strlen(request));
    int n=1;
    for(x = 0; (n = read(s, response + x, 999999 - x)) > 0; x += n);
    if(n == -1) {
        perror("Read fallita");
        return 1;
    }
    response[x] = 0;
    
    printf("%s\n", response);
    printf("Tot bytes = %d\n", x);
    return 0;
}

