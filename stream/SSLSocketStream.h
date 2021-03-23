#pragma once

#include "Stream.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

class SSLSocketStream : public Stream {
public:
  SSLSocketStream(socket_t sock, SSL *ssl, time_t read_timeout_sec,
                  time_t read_timeout_usec, time_t write_timeout_sec,
                  time_t write_timeout_usec);

  virtual ~SSLSocketStream() override;

  bool is_readable() const override;
  bool is_writable() const override;
  ssize_t read(char *ptr, size_t size) override;
  ssize_t write(const char *ptr, size_t size) override;
  void get_remote_ip_and_port(std::string &ip, int &port) const override;
  socket_t socket() const override;

private:
  socket_t m_sock;
  SSL *m_ssl;
  time_t m_readTimeoutSec;
  time_t m_readTimeoutUsec;
  time_t m_writeTimeoutSec;
  time_t m_writeTimeoutUsec;
};