//Riassunto di HTTP1.1, CONTENT-LENGTH E CHUNKED
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // atoi(): DA stringa A int (base 10) + strtol(): DA stringa A long con base specificata (es. 16 per esadecimale)
#include <arpa/inet.h>  // htons()

struct headers {
    char *n;  
    char *v;  
};
struct headers h[100]; 

// Funzione per confrontare due stringhe senza distinzione tra maiuscole/minuscole
// Restituisce 1 se sono uguali (case-insensitive), altrimenti 0
int header_equals(const char *a, const char *b) {
    while (*a && *b) {
        if ((*a | 0x20) != (*b | 0x20)) return 0;  // Converte entrambe in minuscolo e confronta
        a++; b++;
    }
    return *a == *b;  // Assicura che entrambe finiscano nello stesso momento
}

// Formato del body con Transfer-Encoding: chunked:
// <dimensione_chunk_in_esadecimale>\r\n
// <dati_chunk>\r\n
// <dimensione_chunk_in_esadecimale>\r\n
// <dati_chunk>\r\n
// ...
// 0\r\n
// \r\n

// Funzione per leggere un body HTTP codificato con Transfer-Encoding: chunked
// Legge i chunk uno alla volta, decodifica la lunghezza, e li concatena nel buffer
int read_chunked_body(int sockfd, char *buffer, int max_size) {
    int total_read = 0;          // Totale di byte letti
    char line[100];              // Buffer temporaneo per la linea con la lunghezza del chunk
    int len;
    while (1) {
        // Legge la linea che contiene la dimensione del chunk in esadecimale
        int i = 0;
        char c;
        while (i < sizeof(line) - 1 && read(sockfd, &c, 1) == 1) {
            if (c == '\n' && i > 0 && line[i-1] == '\r') break;  // Fine riga: \r\n
            line[i++] = c;
        }
        line[i] = 0;  // Terminazione stringa

        // Converte la stringa esadecimale in intero
        len = (int) strtol(line, NULL, 16);
        if (len == 0) break;   // Chunk finale: lunghezza 0 indica fine trasmissione

        // Controlla se il chunk supera la dimensione massima del buffer
        if (total_read + len >= max_size) {
            fprintf(stderr, "Buffer overflow nella lettura chunked\n");
            return -1;
        }

        // Legge effettivamente i byte del chunk: "len" byte dalla socket
        int n, r;
        for (r = 0; r < len; r += n) {
            n = read(sockfd, buffer + total_read + r, len - r);
            if (n <= 0) return -1;  // Errore di lettura o connessione chiusa prematuramente
        }

        total_read += len;// Aggiorna il contatore totale dei byte letti

        // Legge i due caratteri di fine chunk \r\n che separano i chunk
        read(sockfd, &c, 1); // legge '\r'
        read(sockfd, &c, 1); // legge '\n'
    }
    buffer[total_read] = 0;  // Terminatore stringa
    return total_read;       // Ritorna il numero totale di byte letti
}
// Funzione per cercare il valore di un header dato il nome
// Restituisce un puntatore al valore, o NULL se non trovato
char* find_header_value(const char *name) {
    for (int i = 0; h[i].n; i++) {
        if (header_equals(h[i].n, name)) {
            // Salta eventuali spazi iniziali nel valore
            while (*h[i].v == ' ') h[i].v++;
            return h[i].v;
        }
    }
    return NULL;  // Header non trovato
}

int main() {
    int primiduepuntidopocrlf = 0;
    int i, j;
    char *request = "GET /index.html HTTP/1.1\r\nHost: www.google.com\r\nConnection: keep-alive\r\n\r\n";
    char head[10000];
    char entity_body[1000000];
    unsigned char *p;
    struct sockaddr_in server;
    int n = 1, x = 0, s = 0;
    if (-1 == (s = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("Socket fallita");
        return 1;
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    p = (unsigned char *) &server.sin_addr.s_addr;
    p[0] = 127; p[1] = 0; p[2] = 0; p[3] = 1;
 /*//PER GOOGLE
    server.sin_port = htons(80);
    p = (unsigned char *) &server.sin_addr.s_addr;
    p[0] = 216; p[1] = 58; p[2] = 213; p[3] = 4;
 */
    if (-1 == connect(s, (struct sockaddr *) &server, sizeof(struct sockaddr_in))) {
        perror("Connect fallita");
        return 1;
    }
    write(s, request, strlen(request));
    j = 0;
    h[j].n = head;
    for (i = 0; read(s, head + i, 1) > 0; i++) {
        if (head[i] == ':' && primiduepuntidopocrlf) {
            h[j].v = head + i + 1;
            head[i] = 0;
            primiduepuntidopocrlf = 0;
        }
        if ((head[i] == '\n') && (head[i-1] == '\r')) {
            primiduepuntidopocrlf = 1;
            head[i-1] = 0;
            if (h[j].n[0] == 0)
                break;
            h[++j].n = head + i + 1;
        }
    }
    h[++j].n = NULL;  // marcatore fine lista headers
    for (i = 0; h[i].n; i++)
        printf("Nome: %s ha come Valore: %s\n", h[i].n, h[i].v);

    // Gestione Content-Length o Transfer-Encoding: chunked
    char *cl = find_header_value("Content-Length");
    char *te = find_header_value("Transfer-Encoding");
    if (cl) {
        printf("\n\nLettura modalità: Content-Length\n\n");
        // Se presente Content-Length, leggi numero esatto di byte
        int length = atoi(cl);
        int n, r;
        for (r = 0; r < length; r += n) {
            n = read(s, entity_body + r, length - r);
            if (n <= 0) {
                perror("Errore durante la read con Content-Length");
                return 1;
            }
        }
        x = length;
        entity_body[x] = 0;
    } else if (te && strstr(te, "chunked")) {
        printf("\n\nLettura modalità: Transfer-Encoding: chunked\n\n");
        // Se Transfer-Encoding: chunked
        x = read_chunked_body(s, entity_body, sizeof(entity_body) - 1);
        if (x < 0) {
            perror("Errore nella lettura chunked");
            return 1;
        }
    } else {
        // Fallback: leggi tutto fino a chiusura (non raccomandato in HTTP/1.1 senza Connection: close)
        for (x = 0; 0 < (n = read(s, entity_body + x, 999999 - x)); x += n);
        if (n == -1) {
            perror("Read fallita");
            return 1;
        }
        entity_body[x] = 0;
    }
    printf("%s\n", entity_body);
    printf("Tot bytes = %d\n", x);
    return 0;
}
