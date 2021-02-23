#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>

class SslContext {
public:
    SslContext();
    ~SslContext();

    int Init();
    int Term();
protected:
    SSL *m_ssl = nullptr;
    SSL_CTX *m_sslContext = nullptr;
};