#include "HttpsClient.h"
#include "../stream/SSLSocketStream.h"
#include "../ssl/SslContext.h"


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
}