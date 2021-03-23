#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>

class SslContext {
public:
    SslContext();
    ~SslContext();

    int Init(void);
    void Term(void);

    SSL * GetSSL(void) const {return m_ssl;}
protected:
    SSL *m_ssl = nullptr;
    SSL_CTX *m_sslContext = nullptr;
};