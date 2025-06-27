/*
 * 30. Server HTTP con risposta fissa
 * - Estendi il server precedente per inviare una risposta HTTP/1.1
 *   con intestazioni corrette e un corpo HTML statico.
 * - Obiettivi: generazione di risposta HTTP con Content-Type, Content-Length,
 *  uso di write() su socket.
 */
#include <unistd.h>            // write(), close(), read()
#include <netinet/in.h>        // struct sockaddr_in, htons(), INADDR_ANY
#include <netinet/ip.h>        // IPPROTO_TCP (non usato direttamente, ma spesso incluso con in.h)
#include <sys/types.h>         // tipo socklen_t, size_t (anche se alcuni sono inclusi altrove)
#include <sys/socket.h>        // socket(), bind(), listen(), accept(), setsockopt(), struct sockaddr
#include <stdio.h>             // printf(), perror(), snprintf(), FILE, fopen(), fread(), fclose()
#include <string.h>            // strlen(), strcpy(), memset(), strtok(), strstr()
#include <stdlib.h>            // exit(), EXIT_FAILURE, malloc(), free() (se servissero)
#include <arpa/inet.h>         // inet_ntoa(), ntohs()


#define PORT 8080 //sopra il 1023, sotto sono "privilegiate", 80 per http, 443 per https, per usare serve root.
#define BUFFER_SIZE 1024
//uso un client per accettare le connessioni, e un client per la comunicazione
int main() {
    int server_fd; //file descriptor del socket in ASCOLTO --> usato per ACCETTARE le connessioni in ingresso
    int client_fd; //file descriptor del socket CONNESSO --> usato per COMUNICARE con una connessione dedicata (post accept())
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    char *path = NULL; // aggiunto: dichiarazione fuori dal ciclo di parsing

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
        path = strtok(NULL, " ");               // Estrae il path richiesto (es. /, /pagina.html) da DOV'ERI a SPAZIO
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

    // Controlla se il path è stato estratto correttamente
    if (path == NULL) {
        fprintf(stderr, "Richiesta malformata (path nullo)\n");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    // INIZIO INVIO DELL'ENTITY BODY
    printf("\n\nProcedo ad inviare il file richiesto: %s\n\n", path);

    // Se il path inizia con '/', lo rimuove per ottenere il nome file locale
    if (path[0] == '/') path++;
    // Se il path è vuoto (es. richiesta "/"), viene servito il file di default "index.html"
    if (strlen(path) == 0) path = "index.html";
    // Tenta di aprire il file richiesto in modalità lettura testo ("r")
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        // Se il file non esiste o non è accessibile, prepara una risposta HTTP 404
        // Corpo HTML statico da inviare per errore 404
        const char *body_404 = "<html><body><h1>404 Not Found</h1></body></html>";
        // Buffer che conterrà l'intera risposta HTTP da inviare
        char response[BUFFER_SIZE];
        // Costruisce la risposta HTTP:
        // - Status line: "HTTP/1.1 404 Not Found"
        // - Header: Content-Type, Content-Length, Connection
        // - Corpo: contenuto HTML statico
        snprintf(response, sizeof(response),
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Content-Length: %lu\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s", strlen(body_404), body_404);

        // Invia l'intera risposta 404 al client tramite il socket connesso
        write(client_fd, response, strlen(response));
    } else {
        // Il file esiste: prepara e invia una risposta HTTP 200 OK con il contenuto del file
        // Si posiziona alla fine del file per calcolare la lunghezza in byte
        fseek(fp, 0, SEEK_END);
        // ftell restituisce la posizione corrente del file pointer → corrisponde alla dimensione del file
        long filesize = ftell(fp);

        // Riporta il file pointer all'inizio per poter leggere i dati
        rewind(fp);

        // Prepara l'header HTTP:
        // - Status line: "HTTP/1.1 200 OK"
        // - Content-Type: assume text/html
        // - Content-Length: dimensione effettiva del file
        // - Connection: close (connessione non mantenuta)
        char header[BUFFER_SIZE];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n", filesize);

        // Invia l'header HTTP al client
        write(client_fd, header, strlen(header));

        // Buffer temporaneo per la lettura dei blocchi di file
        char file_buffer[BUFFER_SIZE];
        size_t bytes_read;

        // Legge il file in blocchi da BUFFER_SIZE byte e li invia al client
        while ((bytes_read = fread(file_buffer, 1, BUFFER_SIZE, fp)) > 0) {
            write(client_fd, file_buffer, bytes_read);
        }

        // Chiude il file dopo averlo completamente trasmesso
        fclose(fp);
    }


// 7. Chiudi connessioni
printf("\n\nChiudo le connessioni");
close(client_fd);
close(server_fd);

return 0;
}
