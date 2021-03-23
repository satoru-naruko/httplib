#include "HttpsClient.h"

#include <stream/SSLSocketStream.h>
#include <ssl/SslContext.h>

HttpsClient::HttpsClient(std::string &host, SslContext *sslCtx, int port)
:
m_ssl(sslCtx),
m_socket(host, port)
{
    m_host = host;
    m_port = port;
}

HttpsClient::~HttpsClient()
{
    if (m_stream){
        delete m_stream;    
    }
}

int 
HttpsClient::Get(std::string &path, std::function<int()> resultCallback)
{
    int ret = m_socket.Connect();
    if ( ret < 0)
    {
        printf("Connect Error ret = 0x%x\n",ret);
        return EXIT_FAILURE;
    }

    if (m_stream == nullptr){
        m_stream = new SSLSocketStream(m_socket.GetSocket(), m_ssl->GetSSL(), 10, 0, 10, 0);
    }

    int err = SSL_set_fd(m_ssl->GetSSL(), m_socket.GetSocket());
    SSL_connect(m_ssl->GetSSL());

    char msg[2048] = {};
    sprintf(msg, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: Close\r\n\r\n", path.c_str(), m_host.c_str());

    m_stream->write(msg, strlen(msg));

    size_t read_size = 0;
    char buf[1024]={};
    do {
        read_size = m_stream->read( buf, 1024);
        write(1, buf, read_size);
        memset(buf, 0, sizeof(buf));
    } while(read_size > 0);

}