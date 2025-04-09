#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

//======================================================================
int error(int ern, const char *err) {
    perror(err);
    return ern;
}
//======================================================================
int main(void) {
    /* Ignore SIGPIPE signal */
    signal(SIGPIPE, SIG_IGN);

    /* zmienne predefiniowane *****************************************/
    int port = 12345;
    char host[] = "127.0.0.1";
    /* zmienne niezainicjowane ****************************************/
    int fd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    /* tworzymy gniazdo ***********************************************/
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return error(1, "socket()");

    /* pobieramy informacje o serwerze ********************************/
    server = gethostbyname(host);
    if (server == NULL)
        return error(2, "gethostbyname()");

    /* zapelniamy strukture zerami (inicjujemy) ***********************/
    bzero((char *)&serv_addr, sizeof(serv_addr));

    /* przypisujemy parametry *****************************************/
    serv_addr.sin_family = AF_INET;         // typ gniazda
    serv_addr.sin_port = htons(port);       // port
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    /* nawiazujemy polaczenie *****************************************/
    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        return error(3, "connect()");

    /* wysyłamy numer PID *********************************************/
    pid_t pid = getpid();
    printf("PID: %d\n", pid);
    snprintf(buffer, sizeof(buffer), "%005d", pid);
    n = write(fd, buffer, 5);
    if (n < 0)
        return error(4, "write()");

    /* odczytujemy dane z STDIN i wysylamy do serwera *****************/
    while (1) {
        int ch = getchar();
        if (ch == EOF) break; // koniec wejścia
        //if (ch == '\n') continue; // pomijamy znak nowej linii
        n = write(fd, &ch, 1);
        if (n < 0)
            return error(4, "write()");
    }

    /* konczymy polaczenie ********************************************/
    close(fd);

    //------------------------------------------------------------------
    return 0;
}
//======================================================================