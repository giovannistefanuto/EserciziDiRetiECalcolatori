#include <unistd.h>       // write(), close(), read()
#include <netinet/in.h>   // struct sockaddr_in, htons(), INADDR_ANY
#include <netinet/ip.h>   // IPPROTO_TCP (non usato direttamente, ma spesso incluso con in.h)
#include <sys/types.h>    // tipo socklen_t, size_t (anche se alcuni sono inclusi altrove)
#include <sys/socket.h>   // socket(), bind(), listen(), accept(), setsockopt(), struct sockaddr
#include <stdio.h>        // printf(), perror(), snprintf(), FILE, fopen(), fread(), fclose()
#include <string.h>       // strlen(), strcpy(), memset(), strtok(), strstr()
#include <stdlib.h>       // exit(), EXIT_FAILURE, malloc(), free() (se servissero)
#include <arpa/inet.h>    // inet_ntoa(), ntohs()


#define PORT 8080 //sopra il 1023, sotto sono "privilegiate", 80 per http, 443 per https, per usare serve root.
#define BUFFER_SIZE 1024

// --- Prototipi delle Funzioni ---
int setup_server(int port);
void handle_client(int client_fd);
void send_404_response(int client_fd);
void send_file_response(int client_fd, const char* path);


//uso un client per accettare le connessioni, e un client per la comunicazione
int main() {
    // 1. Crea il socket TCP e mettilo in ascolto
    int server_fd = setup_server(PORT);

    printf("Server in ascolto sulla porta %d...\n", PORT);

    // Ciclo infinito per accettare client in sequenza
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd; //file descriptor del socket CONNESSO --> usato per COMUNICARE con una connessione dedicata (post accept())

        // 5. Accept: accetto un nuovo client sul socket per la comunicazione
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Errore nell'accept, continuo ad ascoltare");
            continue; // Se l'accept fallisce, non termina il server, ma prova ad accettare il prossimo
        }

        printf("\n----------------------------------------\n");
        printf("Connessione accettata da %s:%d\n",
               inet_ntoa(client_addr.sin_addr), //converte indirizzo IPV4 da binario a stringa (inversa di inet_addr)
               ntohs(client_addr.sin_port));    //da big endian a little endian

        // Gestisce l'intera comunicazione con il client
        handle_client(client_fd);

        // 7. Chiudi la connessione con il client corrente
        printf("Chiudo la connessione con il client.\n");
        printf("----------------------------------------\n\n");
        close(client_fd);
    }

    // Questa parte non verrà mai raggiunta in questo esempio,
    // ma in un server reale si chiuderebbe su un segnale (es. CTRL+C)
    close(server_fd);

    return 0;
}

/**
 * @brief Imposta il socket del server, esegue il bind e lo mette in listen.
 * @param port La porta su cui ascoltare.
 * @return Il file descriptor del socket di ascolto.
 */
int setup_server(int port) {
    int server_fd; //file descriptor del socket in ASCOLTO --> usato per ACCETTARE le connessioni in ingresso
    struct sockaddr_in server_addr;

    // 1. Crea il socket TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Errore nella creazione del socket");
        exit(EXIT_FAILURE);
    }

    //per poter riutilizzare la ip:porta (locale) invece di aspettare il TIME WAIT
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Errore in setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 2. Imposta indirizzo e porta
    memset(&server_addr, 0, sizeof(server_addr)); // azzera struttura
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // accetta connessioni da qualsiasi IP di questa macchina
    server_addr.sin_port = htons(port);

    // 3. Bind: dire all'SO: "questo socket sarà il mio endpoint di ascolto sulla porta X, sull'interfaccia Y"
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore nel bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. Listen: mi metto in ascolto sul socket server_fd, con massimo 1 richiesta pendente.
    // In un server reale, il backlog (secondo argomento) sarebbe maggiore (es. 10).
    if (listen(server_fd, 1) < 0) {
        perror("Errore nel listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

/**
 * @brief Gestisce una singola connessione client: riceve, parsa e risponde.
 * @param client_fd Il file descriptor del socket del client connesso.
 */
void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    char* path = NULL; // aggiunto: dichiarazione fuori dal ciclo di parsing

    // 6. Ricezione della richiesta del Client.
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    //recv legge dati ricevuti, li mette nel buffer, restituisce quanti byte ha letto.
    //recv(int sockfd, void *buf, size_t len, int flags);
    if (bytes_received <= 0) {
        if (bytes_received == 0) {
            printf("Il client ha chiuso la connessione.\n");
        } else {
            perror("Errore nella ricezione");
        }
        return; // Termina la gestione di questo client
    }
    buffer[bytes_received] = '\0'; // metto terminatore stringa
    printf("Richiesta ricevuta:\n%s\n", buffer);


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
        // Potremmo inviare una risposta 400 Bad Request qui, ma per ora chiudiamo.
        return;
    }

    // Se il path inizia con '/', lo rimuove per ottenere il nome file locale
    char* effective_path = path;
    if (effective_path[0] == '/') {
        effective_path++;
    }
    // Se il path è vuoto (es. richiesta "/"), viene servito il file di default "index.html"
    if (strlen(effective_path) == 0) {
        effective_path = "index.html";
    }

    // INIZIO INVIO DELL'ENTITY BODY
    printf("\n\nProcedo ad inviare il file richiesto: %s\n\n", effective_path);
    
    // Tenta di aprire il file richiesto in modalità lettura binaria ("rb")
    FILE *fp = fopen(effective_path, "rb");
    if (fp == NULL) {
        // Se il file non esiste o non è accessibile, prepara una risposta HTTP 404
        send_404_response(client_fd);
    } else {
        // Il file esiste: prepara e invia una risposta HTTP 200 OK con il contenuto del file
        send_file_response(client_fd, effective_path);
        // Chiude il file dopo averlo completamente trasmesso
        fclose(fp);
    }
}

/**
 * @brief Invia una risposta HTTP 404 Not Found al client.
 * @param client_fd Il file descriptor del client.
 */
void send_404_response(int client_fd) {
    // Corpo HTML statico da inviare per errore 404
    const char *body_404 = "<html><body><h1>404 Not Found</h1><p>The requested resource could not be found.</p></body></html>";
    // Buffer che conterrà l'intera risposta HTTP da inviare
    char response[BUFFER_SIZE];
    // Costruisce la risposta HTTP:
    // - Status line: "HTTP/1.1 404 Not Found"
    // - Header: Content-Type, Content-Length, Connection
    // - Corpo: contenuto HTML statico
    snprintf(response, sizeof(response),
             "HTTP/1.1 404 Not Found\r\n"
             "Content-Type: text/html; charset=UTF-8\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s", strlen(body_404), body_404);

    // Invia l'intera risposta 404 al client tramite il socket connesso
    write(client_fd, response, strlen(response));
    printf("Risposta 404 Not Found inviata.\n");
}

/**
 * @brief Invia una risposta HTTP 200 OK con il contenuto di un file.
 * @param client_fd Il file descriptor del client.
 * @param path Il percorso del file da inviare.
 */
void send_file_response(int client_fd, const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) { // Doppia sicurezza, anche se già controllato prima
        send_404_response(client_fd);
        return;
    }

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
    printf("Header 200 OK inviato per %s (size: %ld bytes).\n", path, filesize);

    // Buffer temporaneo per la lettura dei blocchi di file
    char file_buffer[BUFFER_SIZE];
    size_t bytes_read;

    // Legge il file in blocchi e li invia al client
    while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), fp)) > 0) {
        if (write(client_fd, file_buffer, bytes_read) < 0) {
            perror("Errore durante l'invio del file");
            break; // Esce dal ciclo se il client si disconnette
        }
    }
    printf("File %s inviato completamente.\n", path);
    
    fclose(fp);
}
