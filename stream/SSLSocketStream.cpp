#include "SSLSocketStream.h"

#include <array>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <signal.h>
#include <netinet/in.h>

template <typename T>
ssize_t handle_EINTR(T fn) {
  ssize_t res = false;
  while (true) {
    res = fn();
    if (res < 0 && errno == EINTR) { continue; }
    break;
  }
  return res;
}

ssize_t poll_read(socket_t sock, time_t sec, time_t usec) {
    struct pollfd pfd_read;
    pfd_read.fd = sock;
    pfd_read.events = POLLIN;
    auto timeout = static_cast<int>(sec * 1000 + usec / 1000);
    return ::handle_EINTR([&]() { return poll(&pfd_read, 1, timeout); });
}

ssize_t poll_write(socket_t sock, time_t sec, time_t usec) {
  struct pollfd pfd_read;
  pfd_read.fd = sock;
  pfd_read.events = POLLOUT;

  auto timeout = static_cast<int>(sec * 1000 + usec / 1000);

  return ::handle_EINTR([&]() { return poll(&pfd_read, 1, timeout); });
}

void get_remote_ip_and_port(
    const struct sockaddr_storage &addr,
    socklen_t addr_len, 
    std::string &ip,
    int &port
 )
 {
  if (addr.ss_family == AF_INET) {
    port = ntohs(reinterpret_cast<const struct sockaddr_in *>(&addr)->sin_port);
  } else if (addr.ss_family == AF_INET6) {
    port =
        ntohs(reinterpret_cast<const struct sockaddr_in6 *>(&addr)->sin6_port);
  }

  std::array<char, NI_MAXHOST> ipstr;
  if (!getnameinfo(reinterpret_cast<const struct sockaddr *>(&addr), addr_len,
                   ipstr.data(), static_cast<socklen_t>(ipstr.size()), nullptr,
                   0, NI_NUMERICHOST)) {
    ip = ipstr.data();
  }
}

void get_remote_ip_and_port(socket_t sock, std::string &ip, int &port) {
  struct sockaddr_storage addr;
  socklen_t addr_len = sizeof(addr);

  if (!getpeername(sock, reinterpret_cast<struct sockaddr *>(&addr),
                   &addr_len)) {
    get_remote_ip_and_port(addr, addr_len, ip, port);
  }
}

// SSL socket stream implementation
SSLSocketStream::SSLSocketStream(socket_t sock, SSL *ssl,
                                        time_t read_timeout_sec,
                                        time_t read_timeout_usec,
                                        time_t write_timeout_sec,
                                        time_t write_timeout_usec)
    : m_sock(sock), m_ssl(ssl), m_readTimeoutSec(read_timeout_sec),
      m_readTimeoutUsec(read_timeout_usec),
      m_writeTimeoutSec(write_timeout_sec),
      m_writeTimeoutUsec(write_timeout_usec) {
  SSL_clear_mode(ssl, SSL_MODE_AUTO_RETRY);
}

inline SSLSocketStream::~SSLSocketStream() {}

bool SSLSocketStream::is_readable() const {
  return poll_read(m_sock, m_readTimeoutSec, m_readTimeoutUsec) > 0;
}

bool SSLSocketStream::is_writable() const {
  return poll_write(m_sock, m_writeTimeoutSec, m_writeTimeoutUsec) >0;
}

ssize_t SSLSocketStream::read(char *ptr, size_t size) {
  if (SSL_pending(m_ssl) > 0) {
    return SSL_read(m_ssl, ptr, static_cast<int>(size));
  } else if (is_readable()) {
    auto ret = SSL_read(m_ssl, ptr, static_cast<int>(size));
    if (ret < 0) {
      auto err = SSL_get_error(m_ssl, ret);
      while (err == SSL_ERROR_WANT_READ) {
        if (SSL_pending(m_ssl) > 0) {
          return SSL_read(m_ssl, ptr, static_cast<int>(size));
        } else if (is_readable()) {
          ret = SSL_read(m_ssl, ptr, static_cast<int>(size));
          if (ret >= 0) { return ret; }
          err = SSL_get_error(m_ssl, ret);
        } else {
          return -1;
        }
      }
    }
    return ret;
  }
  return -1;
}

inline ssize_t SSLSocketStream::write(const char *ptr, size_t size) {
  if (is_writable()) { return SSL_write(m_ssl, ptr, static_cast<int>(size)); }
  return -1;
}

void SSLSocketStream::get_remote_ip_and_port(std::string &ip,
                                                    int &port) const {
  ::get_remote_ip_and_port(m_sock, ip, port);
}

inline socket_t SSLSocketStream::socket() const { return m_sock; }