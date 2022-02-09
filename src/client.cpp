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

void receive_from_server(int);

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

    char username[40] = { };
    std::cout << "Your Username: ";
    std::cin >> username;

    send(client_sockfd, (void*)username, 40, 0);

    std::thread server_listener(receive_from_server, client_sockfd);

    char send_buffer[MAX_BUF_SIZE] = { };
    while (true) {
        std::string str_buf;
        std::getline(std::cin, str_buf);
        std::strcpy(send_buffer, str_buf.c_str());
        send(client_sockfd, (void*)send_buffer, 1000, 0);
        memset((void*)send_buffer, 0, sizeof(send_buffer));
    }
}

void receive_from_server(int client_sockfd) {
    char recv_buffer[MAX_BUF_SIZE] = { };
    while (true) {
        recv(client_sockfd, (void*)recv_buffer, 1000, 0);
        std::cout << recv_buffer << std::endl;
    }
}
