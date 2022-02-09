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

#include <common_macros.hpp>

class Server {
public:
    Server() = default;

    void run();
private:
    void message_all(const char*, int);
    void handle_client(client_info);
private:
    int server_sockfd;
    int min_available_id = 1;
    std::vector<std::pair<client_info, int>> clients;
};

////////////////////////////////////////////////
void Server::run() {
    server_sockfd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    std::memset((void*)&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton("127.0.0.1", &(server_addr.sin_addr));

    int bind_status = bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

    if (bind_status == -1) {
        std::cerr << "Unable to bind on port " << PORT << std::endl;
        exit(bind_status);
    }

    while (true) {
        listen(server_sockfd, BACKLOG);

        struct sockaddr_in client_addr;
        std::memset((void*)&client_addr, 0, sizeof(struct sockaddr_in));
        socklen_t client_addr_len = sizeof(struct sockaddr_in);

        int conn_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &client_addr_len);

        if (conn_sockfd == -1) {
            std::cerr << "Error while accepting connection from client\nExiting...\n";
            exit(conn_sockfd);
        }

        // do things with this newly obtained file descriptor
        client_info client = {conn_sockfd, client_addr};
        clients.push_back(std::make_pair(client, min_available_id++));

        std::thread client_thread([this, client]{handle_client(client);});
        client_thread.detach();
    }
}

void Server::handle_client(client_info client) {
    std::cout << "Connection accepted from client\n";
    std::cout << "IP: " << inet_ntoa(client.client_addr.sin_addr) << std::endl;
    std::cout << "PORT: " << ntohs(client.client_addr.sin_port) << std::endl;

    char send_buffer[MAX_BUF_SIZE] = { };
    char recv_buffer[MAX_BUF_SIZE] = { };

    while (true) {
        // read from client and reply
    }
}

void Server::message_all(const char* msg, int sender_id) {
    for (auto p: clients) {
        if (sender_id == p.second) continue;
        int conn_sockfd = p.first.conn_sockfd;

        char stringified_id[10] = { };
        std::sprintf(stringified_id, "%d: ", sender_id);

        send(conn_sockfd, (void*)msg, std::strlen(msg) + 1, 0);
    }
}
