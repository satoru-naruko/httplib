#include "SslContext.h"

SslContext::SslContext()
{   
}

SslContext::~SslContext()
{
}

int 
SslContext::Init(void)
{
    SSL_load_error_strings();
    SSL_library_init();

    m_sslContext = SSL_CTX_new(SSLv23_client_method());
    if(m_sslContext == nullptr){
        return -1;
    }
    m_ssl = SSL_new(m_sslContext);
    if (m_ssl == nullptr) {
        return -1;
    }

    SSL_CTX_set_options(m_sslContext ,
        SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 |
        SSL_OP_NO_COMPRESSION |
        SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);

    return 0;
}

void
SslContext::Term(void)
{    
    if (m_ssl){
        SSL_shutdown(m_ssl);
        SSL_free(m_ssl);
    }

    if (m_sslContext){
        SSL_CTX_free(m_sslContext);
    }

    m_ssl = nullptr;
    m_sslContext = nullptr;
    
}
