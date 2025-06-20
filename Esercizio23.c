// 23. Analisi dello stato TIME_WAIT
// - Connettiti a un server HTTP, chiudi correttamente la connessione,
//   e usa il comando `netstat -tn` per analizzare lo stato TIME_WAIT.
// - Obiettivi: terminazione TCP, diagnostica rete.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>  // aggiungi questa
#include <string.h>
int main() {
   
    char* request = "GET /\r\n";
    char response[1000000];
    int x = 1;
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




    //GIà CHE CI SIAMO, AGGIUNGIAMO UTILIZZO DI DNS
struct hostent *he = gethostbyname("google.com");
if (he == NULL) {
    perror("gethostbyname fallita");
    close(s);
    return 1;
}
server.sin_addr = *(struct in_addr *)he->h_addr;
   // server.sin_addr.s_addr = inet_addr("216.58.213.4");
   //il resto è uguale
    if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Connection fallita");
        close(s);
        return 1;
    }
    printf("Connessione riuscita a 216.58.213.4:80, google.com\n");


    printf("\n\n\n\n\nSEPARATORE DELLE PARTI\n\n\n\n\n");
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
    shutdown(s, SHUT_WR); // disabilita l'invio di dati (invio FIN)
    close(s);  
    return 0;
}

//con wsl 2 non va questa cosa, usare invece cmd normale ed usare:
//netstat -n -p TCP | findstr TIME_WAIT
