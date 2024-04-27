#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <cstring> 
#define PORT 8080  

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
    bind(server_fd, address, address.sin_len);
    if (listen(server_fd, 3) < 0) { 
        perror("In listen"); 
        exit(EXIT_FAILURE); 
    }
    if (accept(server_fd, address, address.sin_len) < 0) {
        perror("In accept");            
        exit(EXIT_FAILURE);        
    }
        return 0; // Added for main to return an integer
}