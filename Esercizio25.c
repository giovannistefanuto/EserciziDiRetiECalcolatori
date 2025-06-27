// 25. Costruzione manuale di una richiesta HTTP
// - Input: metodo (es. "GET", "POST") e URI (es. "/index.html")
// - Costruire una richiesta HTTP correttamente formattata (es. "GET /index.html HTTP/1.0\r\n\r\n")
// - Obiettivi: gestione delle stringhe in C, formattazione corretta secondo il protocollo HTTP,
//   e uso corretto della memoria (allocazione dinamica se necessario)
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
unsigned short int invert_byte_order(unsigned short a) {
    unsigned char *p, t;
    p = (unsigned char *) &a;  // Ottiene il puntatore ai byte della variabile 'a'
    t = *p;                // Salva il primo byte in una variabile temporanea
    *p = *(p+1);           // Sostituisce il primo byte con il secondo
    *(p+1) = t;            // Assegna il byte salvato al secondo byte
    return a;              // Restituisce il valore con i byte invertiti
}

// Struttura per rappresentare un header HTTP.
// Ogni header è costituito da un nome e da un valore, 
// entrambi gestiti come stringhe (puntatori a char).
struct headers {
    char *n;  // Puntatore alla stringa contenente il nome dell'header
    char *v;  // Puntatore alla stringa contenente il valore dell'header
};

struct headers h[100];  // Array per memorizzare fino a 100 header

int main() {
    int primiduepuntidopocrlf;  
    // Flag per indicare se, dopo aver letto un CRLF,
    // siamo pronti a cercare il separatore ':' nell'header.
    
    char ch;  // Variabile temporanea (non utilizzata in questo esempio)
    int i, j;
    
    // Stringa contenente la richiesta HTTP/1.0 da inviare al server.
    char *request = "GET /index.html HTTP/1.0 \r\n\r\n";
    
    char head[10000];          
    // Buffer per memorizzare gli header della risposta.
    
    char entity_body[1000000]; 
    // Buffer per memorizzare il corpo (entity body) della risposta.
    
    unsigned char *p;  
    struct sockaddr_in server; 
    int n = 1, x = 1, s = 0;   

    // Creazione di un socket TCP.
    if (-1 == (s = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("Socket fallita");
        return 1;
    }

    // Esempio di utilizzo della funzione invert_byte_order.
    // Stampa il risultato in esadecimale per verificare il risultato.
    printf("%x\n", invert_byte_order(0x1234));

    // Configurazione dell'indirizzo del server.
    server.sin_family = AF_INET;              
    // Specifica che si utilizza il protocollo IPv4.
    
    server.sin_port = invert_byte_order(80);    
    // Imposta la porta 80 (HTTP) convertendo l'ordine dei byte.
    
    p = (unsigned char *) &server.sin_addr.s_addr;
    // Imposta l'indirizzo IP del server (in questo esempio: 216.58.213.4).
    p[0] = 216; p[1] = 58; p[2] = 213; p[3] = 4;

    // Connessione al server specificato.
    if (-1 == connect(s, (struct sockaddr *) &server, sizeof(struct sockaddr_in))) {
        perror("Connect fallita");
        return 1;
    }

    // Invio della richiesta HTTP al server.
    write(s, request, strlen(request));

    // Inizializzazione per il parsing degli header:
    // Il buffer 'head' conterrà i dati degli header,
    // mentre l'array 'h' memorizzerà i puntatori ai nomi e valori
    // all'interno del buffer.
    j = 0;            // Inizializza l'indice per gli header.
    h[j].n = head;    // Il primo header inizia all'inizio del buffer 'head'.
    
    // Loop per leggere la risposta dal server, un carattere alla volta,
    // e processare il buffer per separare nomi e valori degli header.
    for (i = 0; read(s, head + i, 1) > 0; i++) {
        // Se il carattere corrente è ':' e il flag è attivo,
        // significa che abbiamo terminato di leggere il nome dell'header.
        if (head[i] == ':' && primiduepuntidopocrlf) {
            h[j].v = head + i + 1;  
            // Imposta il puntatore al valore dell'header.
            
            head[i] = 0;            
            // Sostituisce il carattere ':' con il terminatore di stringa,
            // delimitando così il nome dell'header.
            
            primiduepuntidopocrlf = 0; 
            // Disattiva il flag per evitare di interpretare altri ':' nello stesso header.
        }
        // Se viene rilevato il terminatore di riga (CRLF),
        // significa che abbiamo terminato la lettura dell'header corrente.
        if ((head[i] == '\n') && (head[i-1] == '\r')) {
            primiduepuntidopocrlf = 1;  
            // Riattiva il flag, in quanto dopo CRLF siamo pronti
            // a processare il successivo header.
            
            head[i-1] = 0;             
            // Sostituisce il carattere CR ('\r') con un terminatore di stringa,
            // completando l'header corrente.
            
            // Se il nuovo header risulta essere una stringa vuota,
            // significa che abbiamo incontrato il doppio CRLF e sono finiti gli header.
            if (h[j].n[0] == 0)
                break;
            
            // Incrementa l'indice e imposta il puntatore al nome del nuovo header.
            // L'uso del pre-incremento (++j) garantisce che 'j' venga aggiornato
            // prima di essere utilizzato per l'assegnazione.
            h[++j].n = head + i + 1;
        }
    }

    // Visualizza tutti gli header letti, mostrando nome e valore associato.
    for (i = 0; i < j; i++)
        printf("Nome: %s ha come Valore: %s\n", h[i].n, h[i].v);

    for (x = 0; 0 < (n = read(s, entity_body + x, 999999 - x)); x += n);
    if (n == -1) {
        perror("Read fallita");
        return 1;
    }
    entity_body[x] = 0;

    printf("%s\n", entity_body);
    printf("Tot bytes = %d\n", x);

    return 0;
}