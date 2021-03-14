#pragma once

#include <string>
#include <functional>

#include <stream/Stream.h>
#include <socket/Socket.h>

class HttpClient
{
public:
    HttpClient(std::string &host, int port = 80);
    ~HttpClient();

    int Get(std::string &path, std::function<int(int,int)> resultCallback);
protected:
    Stream *m_stream = nullptr;
    Socket m_socket;
};

HttpClient::HttpClient(std::string &host, int port)
:
m_socket(host, port)
{
}

HttpClient::~HttpClient()
{
    if (m_stream){
        delete m_stream;    
    }
}
