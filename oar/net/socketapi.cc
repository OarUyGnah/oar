#include "socketapi.h"
#include "../Endian.h"
#include "../log.h"
#include "socketapi.h"
#include <arpa/inet.h>
#include <cstdarg>
#include <cstdlib>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace oar {

int socket(int domain, int type, int protocol)
{
    int sock;
    if ((sock = ::socket(domain, type, protocol)) < 0) {
        unix_error("oar::socket error");
    }
    return sock;
}

int connect(int sockfd, const struct sockaddr* addr)
{
    int conn;
    if ((conn = ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)))) < 0) {
        unix_error("oar::connect error");
    }
    return conn;
}

void bind(int sockfd, const struct sockaddr* addr)
{
    if (::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in))) < 0) {
        unix_error("oar::bind error");
    }
}

void listen(int sockfd)
{
    if (::listen(sockfd, SOMAXCONN) < 0) {
        unix_error("oar::listen error");
    }
}

int accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
    int connfd = accept(sockfd, static_cast<struct sockaddr*>(static_cast<void*>(addr)), &addrlen);
    if (connfd < 0) {
        int errnum = errno;
        // unix_error("oar::accept error");
        switch (errnum) {
        case EAGAIN:
        case ECONNABORTED:
        case EINTR:
        case EPROTO: // ???
        case EPERM:
        case EMFILE: // per-process lmit of open file desctiptor ???
            // expected errors
            errno = errnum;
            break;
        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOBUFS:
        case ENOMEM:
        case ENOTSOCK:
        case EOPNOTSUPP:
            unix_error("unexpected error in oar::accept ");
            break;
        default:
            unix_error("unknown error in oar::accept ");
            break;
        }
    }
    return connfd;
}

int set_nonblocking(int sockfd)
{
    int old = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, old | O_NONBLOCK) == -1) {
        close(sockfd);
        unix_error("oar::set_nonblocking error");
        return -1;
    }
    return old;
}

void shutdown(int fd, int how)
{
    if (::shutdown(fd, how) < 0) {
        unix_error("error in oar::shutdown");
    }
}
void shutdownWR(int sockfd)
{
    if (::shutdown(sockfd, SHUT_WR) < 0) {
        unix_error("error in oar::shutdownWR");
    }
}
void shutdownRD(int sockfd)
{
    if (::shutdown(sockfd, SHUT_RD) < 0) {
        unix_error("error in oar::shutdownWR");
    }
}
void shutdownRDWR(int sockfd)
{
    if (::shutdown(sockfd, SHUT_RDWR) < 0) {
        unix_error("error in oar::shutdownWR");
    }
}

void close(int sockfd)
{
    if (::close(sockfd) < 0) {
        unix_error("error in oar::close");
    }
}

void setaddr(const char* ip, uint16_t port, sockaddr_in& addr)
{
    addr.sin_port = hton(port);
    std::cout << "port is " << port << std::endl;
    std::cout << "hton(port) is " << hton(port) << std::endl;

    addr.sin_family = AF_INET;
    if (::inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        unix_error("error in oar::setaddr");
    }
}

int getSockErr(int sockfd)
{
    int optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
        return errno;
    } else {
        return optval;
    }
}

ssize_t read(int sockfd, void* buf, size_t nbytes)
{
    return ::read(sockfd, buf, nbytes);
}

ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t write(int sockfd, const void* buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

void unix_error(const char* msg, ...)
{
    char* buf;
    va_list vl;
    va_start(vl, msg);
    if (vasprintf(&buf, msg, vl) != -1) {
        fprintf(stderr, "%s\n", buf);
        va_end(vl);
    }
    exit(0);
}
}