
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <signal.h>
#include <netinet/in.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

int main(void) {
    int mysocket;
    struct sockaddr_in server;
    struct addrinfo hints, *res;

    SSL *ssl;
    SSL_CTX *ctx;

    char msg[128] = {0};

    const char *host = "localhost";
    const char *path = "/";

    // IPアドレスの解決
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    const char *service = "8080";

    int err = 0;
    if ((err = getaddrinfo(host, service, &hints, &res)) != 0) {
        fprintf(stderr, "Fail to resolve ip address - %d\n", err);
        return EXIT_FAILURE;
    }

    if ((mysocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        fprintf(stderr, "Fail to create a socket.\n");
        freeaddrinfo(res);
        return EXIT_FAILURE;
    }

    err = connect(mysocket, res->ai_addr, res->ai_addrlen);
    if ( err != 0) {
        perror("Connection error ¥n");
        freeaddrinfo(res);
        return EXIT_FAILURE;
    }

    SSL_load_error_strings();
    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());
    ssl = SSL_new(ctx);

    SSL_CTX_set_options(ctx,
                        SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 |
                            SSL_OP_NO_COMPRESSION |
                            SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);

    err = SSL_set_fd(ssl, mysocket);
    SSL_connect(ssl);

    printf("Conntect to %s\n", host);

    sprintf(msg, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\n\r\n", path, host);

    SSL_write(ssl, msg, strlen(msg));

    int buf_size = 256;
    char buf[buf_size];
    int read_size = 0;
    struct pollfd target;
    target.fd = mysocket;
    target.events = POLLIN | POLLERR;
    target.revents = 0;

    bool bExit = false;
    for (;;){
        switch (poll(&target, 1, 1 * 1000))
        {
        case -1:
            if (errno != EINTR){
                perror("poll");
                bExit = true;
            }
            break;
        case 0: // timeout
            break;
                
        default:
        if (target.revents & (POLLIN | POLLERR))
        {
            do {
                read_size = SSL_read(ssl, buf, buf_size);
                write(1, buf, read_size);
                memset(buf, 0, sizeof(buf));
            } while(read_size > 0);
            bExit = true;
        }
        
        break;
        }
        if (bExit){
            break;
        }
    }
    
    freeaddrinfo(res);
    
    SSL_shutdown(ssl);    
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    close(mysocket);

    return EXIT_SUCCESS;
}
