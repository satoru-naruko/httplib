#include "SslContext.h"

SslContext::SslContext()
{   
}

SslContext::~SslContext()
{
    Term();
}

int 
SslContext::Init(void)
{
    SSL_load_error_strings();
    SSL_library_init();

    m_sslContext = SSL_CTX_new(SSLv23_client_method());
    m_ssl = SSL_new(m_sslContext);

    SSL_CTX_set_options(m_sslContext ,
        SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 |
        SSL_OP_NO_COMPRESSION |
        SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);

    return 0;
}

int 
SslContext::Term()
{
    SSL_shutdown(m_ssl);    
    SSL_free(m_ssl);
    SSL_CTX_free(m_sslContext);

    return 0;
}
