#include "Socket.h"

#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

Socket::Socket(std::string &host, int port)
: 
m_host(host),
m_port(port)
{
}

int 
Socket::Connect()
{
    const char *host = m_host.c_str();
    int tmp_socket = 0;
    struct addrinfo hints, *res;

    // IPアドレスの解決
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    auto service = std::to_string(m_port);

    int err = 0;
    if ((err = getaddrinfo(host, service.c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "Fail to resolve ip address - %d\n", err);
        return -1;
    }

    if ((tmp_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        fprintf(stderr, "Fail to create a socket.\n");
        freeaddrinfo(res);
        return -2;
    }

    err = connect(tmp_socket, res->ai_addr, res->ai_addrlen);
    if ( err != 0) {
        perror("Connection error ¥n");
        freeaddrinfo(res);
        return -3;
    }

    freeaddrinfo(res);
    m_socket = tmp_socket;

    return 0;
}

void
Socket::Close(void)
{
    if (m_socket){
       close(m_socket);
       m_socket = 0;
    }
}