#ifndef __OAR_SOCKETAPI_H__
#define __OAR_SOCKETAPI_H__

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

namespace oar {
int socket(int domain, int type = SOCK_STREAM, int protocol = 0);
int connect(int sockfd, const struct sockaddr* addr);
void bind(int sockfd, const struct sockaddr* addr);
void listen(int sockfd);
int accept(int sockfd, struct sockaddr_in* addr);
int set_nonblocking(int sockfd);
void shutdown(int sockfd, int how);
void shutdownWR(int sockfd);
void shutdownRD(int sockfd);
void shutdownRDWR(int sockfd);
void close(int sockfd);

void setaddr(const char* ip, uint16_t port, sockaddr_in& addr);
int getSockErr(int sockfd);
ssize_t read(int fd, void* buf, size_t nbytes);
ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt);
// ssize_t readn(int fd, char* buf, int nbytes);
ssize_t write(int sockfd, const void* buf, size_t count);

void unix_error(const char* msg, ...);
}

#endif