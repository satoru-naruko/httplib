#pragma once

#include <string>

class Socket {
public:
    Socket(std::string &host, int port);
    ~Socket();

    int Connect(void);
    void Close(void);

protected:
    int m_socket = 0;
    std::string m_host;
    int m_port = 0;
};