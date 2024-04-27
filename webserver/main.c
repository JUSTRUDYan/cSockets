#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h> // For close()
#include <pthread.h>
#include <vector>
#include <iostream>

#define PORT 8080  

struct ClientInfo {
    int socket;
    struct sockaddr_in address;
};

std::vector<ClientInfo> clients;

void *responseMessage(void *arg) {
    int client_socket = *((int *)arg);
    const char *message = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>Hello, World!</h1></body></html>\n";
    send(client_socket, message, strlen(message), 0);
    close(client_socket);
    return NULL;
}

void *handleConnection(void *arg) {
    int server_fd = *((int *)arg);
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    
    while (1) {
        int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        ClientInfo new_client;
        new_client.socket = client_socket;
        new_client.address = address;
        printf("New client connected from %s:%d\n", inet_ntoa(new_client.address.sin_addr), ntohs(new_client.address.sin_port));
        clients.push_back(new_client);

        pthread_t tid;
        if (pthread_create(&tid, NULL, responseMessage, (void *)&client_socket) != 0) {
            perror("pthread_create failed");
            close(client_socket);
            continue;
        }

        pthread_detach(tid);
    }

    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address)); 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(PORT);
     
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) { 
        perror("listen failed"); 
        exit(EXIT_FAILURE); 
    }

    pthread_t tid;

    if (pthread_create(&tid, NULL, handleConnection, (void *)&server_fd) != 0) {
        perror("pthread_create failed");
        exit(EXIT_FAILURE);
    }

    pthread_join(tid, NULL);

    close(server_fd);
    return 0; 
}
