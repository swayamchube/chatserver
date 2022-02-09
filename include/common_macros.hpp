#ifndef COMMON_MACROS_HPP
#define COMMON_MACROS_HPP
#define PORT 8080
#define MAX_BUF_SIZE 1024
#define BACKLOG 3

struct client_info {
    int conn_sockfd;
    struct sockaddr_in client_addr;
};

#endif
