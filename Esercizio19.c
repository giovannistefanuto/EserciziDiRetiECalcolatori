/*
 * Gestione dei segnali (SIGINT)
 * • Aggiungi una gestione del segnale SIGINT (Ctrl+C) per chiudere correttamente il socket aperto.
 * • Obiettivi: utilizzo di signal() o sigaction() per intercettare il segnale,
 *   migliorare la robustezza del codice evitando risorse lasciate aperte o stati inconsistenti.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int s; // socket globale per poterlo chiudere nel signal handler

void handler(int signum) {
    printf("\nSIGINT ricevuto. Chiudo il socket %d...\n", s);
    close(s);
    exit(0);
}

int main() {
    struct sockaddr_in server;

    // Registra il signal handler
    signal(SIGINT, handler);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Creazione socket fallita");
        return 1;
    }
    printf("Socket creato: %d\n", s);

    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr.s_addr = inet_addr("96.7.128.198");

    if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Connection fallita");
        close(s);
        return 1;
    }

    printf("Connessione riuscita a 96.7.128.198:80\n");

    // Simula attività (es. attesa)
   pause();  // attende segnale

    return 0;
}

/*
 * Necessario:
 * Includere  #include <signal.h>
 * mettere questo da qualche parte: signal(SIGINT, handler);
 * creare la funzione che gestisce: 
 * void handler(int signum) {
    printf("\nSIGINT ricevuto. Chiudo il socket %d...\n", s);
    close(s);
    exit(0); //in accordo con pause
}
    
// Simula attività (es. attesa)
   pause();  // attende segnale
 */

