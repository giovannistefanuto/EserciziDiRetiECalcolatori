
/*
 * (29) ❑ Server HTTP minimale con parsing base
 * 
 * Scrivi un server TCP che accetta una singola connessione, riceve una richiesta HTTP e:
 *   - stampa i primi 1024 byte ricevuti,
 *   - estrae e stampa metodo e path della prima riga,
 *   - legge e stampa tutte le intestazioni fino alla riga vuota.
 *
 * Obiettivi: uso base di socket TCP, parsing della richiesta HTTP con strtok(),
 *            lettura riga per riga fino a "\r\n\r\n".
 */
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <arpa/inet.h> 

#define PORT 8080 //sopra il 1023, sotto sono "privilegiate", 80 per http, 443 per https, per usare serve root.
#define BUFFER_SIZE 1024
//uso un client per accettare le connessioni, e un client per la comunicazione
int main() {
    int server_fd; //file descriptor del socket in ASCOLTO --> usato per ACCETTARE le connessioni in ingresso
    int client_fd; //file descriptor del socket CONNESSO --> usato per COMUNICARE con una connessione dedicata (post accept())
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 1. Crea il socket TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    // 2. Imposta indirizzo e porta
    memset(&server_addr, 0, sizeof(server_addr)); // azzera struttura
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // accetta connessioni da qualsiasi IP di questa macchina
    server_addr.sin_port = htons(PORT);
    //per poter riutilizzare la ip:porta (locale) invece di aspettare il TIME WAIT
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. Bind: dire all'SO: "questo socket sarà il mio endpoint di ascolto sulla porta X, sull'interfaccia Y"
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore nel bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Listen: mi metto in ascolto sul socket server_fd, con massimo 1 richiesta pendente.
    if (listen(server_fd, 1) < 0) {
        perror("Errore nel listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server in ascolto sulla porta %d...\n", PORT);

    // 5. Accept: accetto un nuovo client sul socket per la comunicazione
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("Errore nell'accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connessione accettata da %s:%d\n",
           inet_ntoa(client_addr.sin_addr), //converte indirizzo IPV4 da binario a stringa (inversa di inet_addr)
           ntohs(client_addr.sin_port));    //da big endian a little endian

    // 6. Ricezione della richiesta del Client.
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    //recv legge dati ricevuti, li mette nel buffer, restituisce quanti byte ha letto.
    //recv(int sockfd, void *buf, size_t len, int flags);
    if (bytes_received < 0) {
        perror("Errore nella ricezione");
    } else {
        buffer[bytes_received] = '\0'; // metto terminatore stringa
        printf("Richiesta ricevuta:\n%s\n", buffer);
    }

// Parsing della richiesta HTTP dopo averla ricevuta nel buffeR
printf("\n\nInizio del parsing\n\n");

char *start = buffer;           // Puntatore all'inizio della parte da analizzare (inizio buffer)
char *end;                      // Verrà usato per trovare il termine di ogni riga
int line_number = 0;            // Contatore per distinguere prima riga dalle intestazioni

// Ciclo che scorre ogni riga della richiesta fino alla fine degli header (\r\n\r\n)
while ((end = strstr(start, "\r\n")) != NULL) {
    *end = '\0';                // Termina la riga corrente sostituendo \r con '\0' (stringa C)

    if (line_number == 0) {
        // La prima riga della richiesta HTTP (es. "GET /index.html HTTP/1.1") ovvero la Status-Line
        char *method = strtok(start, " ");      // Estrae il metodo HTTP (es. GET, POST), da start a SPAZIO
        char *path = strtok(NULL, " ");         // Estrae il path richiesto (es. /, /pagina.html) da DOV'ERI a SPAZIO
        char *version = strtok(NULL, " ");      // Estrae la versione HTTP (opzionale qui) da DOV'ERI a SPAZIO

        // Stampa i valori estratti
        printf("Metodo: %s\n", method);
        printf("Path: %s\n", path);
        if (version == NULL) {
            printf("Versione non specificata, potrebbe essere HTTP/0.9\n");
        }
        else{printf("Version: %s\n", version);}
        
        printf("\n\nHEADER:\n\n");
    } else {
    // Se la riga è vuota, significa che gli header sono finiti
    if (strlen(start) == 0)
        break;

    // Divide header in nome e valore (usa ':' come delimitatore)
    char *header_name = strtok(start, ":");
    char *header_value = strtok(NULL, ""); // prende tutto il resto dopo ':'

    // Se c'è valore, rimuove eventuali spazi iniziali
    if (header_value != NULL) {
        while (*header_value == ' ') header_value++; // skip spazi iniziali
    }

    printf("Intestazione: %s\n", start);
    printf("header_name: %s, header_value: %s\n", header_name, header_value);
    }

    start = end + 2;            // Sposta il puntatore all'inizio della prossima riga (salta "\r\n")
    line_number++;              // Passa alla riga successiva
}

// 7. Chiudi connessioni
close(client_fd);
close(server_fd);

return 0;

}