#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <thread>

// Networking related headers
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <client.hpp>
#include <common_macros.hpp>

int main(int argc, char** argv) {
    int client_sockfd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    std::memset((void*)&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &(server_addr.sin_addr));

    int connect_status = connect(client_sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));

    if (connect_status == -1) {
        std::cerr << "Error while connecting with the server" << std::endl;
        exit(connect_status);
    }

    char recv_buffer[MAX_BUF_SIZE] = { };

    recv(client_sockfd, (void*)recv_buffer, 1000, 0);

    std::cout << "Message from server: ";
    std::cout << recv_buffer << std::endl;
}
