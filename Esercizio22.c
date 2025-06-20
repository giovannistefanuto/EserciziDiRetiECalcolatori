/*22. Funzione di scrittura sicura
• Implementa safe write(int fd, char *buf, int len) che
garantisce l’invio completo di un buffer tramite write().
• Obiettivi: gestione invio parziale, robustezza dell’I/O
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

#include <errno.h>
void safe_write(int socket, char* my_request) {
    int lunghezza = strlen(my_request);
    ssize_t result = 0;

    while (result < lunghezza) {
        ssize_t n = write(socket, my_request + result, lunghezza - result);
        if (n == -1) {
            if (errno == EINTR)
                continue;  // retry if interrupted
            perror("Errore fatale");
            return;
        }
        result += n;
    }
}

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
    char * p = (char*)&response;
    //PARTI NUOVE
    safe_write(s, request);
    //FINE PARTI NUOVE 
    int finale = x;
    while(x){
        x = read(s, p, 1400);
        printf("%s", p);
        printf("\n%d bytes\n", x);
        p = p + x;
        finale += x; 
    }
    *p = 0;
    printf("Tot bytes = %d\n", finale);
   // printf("RISPOSTA TOTALE = %s\n", response);
    return 0;
}

//USARE SU CMD: netstat -anp | grep :80
