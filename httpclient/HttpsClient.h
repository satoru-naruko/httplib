#pragma once

#include <string>
#include <functional>

#include <stream/Stream.h>
#include <socket/Socket.h>


class SslContext;
class HttpsClient
{
public:
    HttpsClient(std::string &host, SslContext *sslCtx,int port = 443);
    ~HttpsClient();

    int Get(std::string &path, std::function<int()> resultCallback);
protected:
    SslContext *m_ssl;
    Stream *m_stream = nullptr;
    Socket m_socket;
    std::string m_host;
    int m_port = 0;
};
