#include "server.h"

#define MAX_SIZE 1024
#define PORT 8080

int main(int argc, char const* argv[]){

    int server_fd;

    //Create server socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    //Bind the server socket
    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address))){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    //Listen to the socket and accept client connections
    listen(server_fd, 5);
    int client_fd = accept(server_fd, nullptr, nullptr);

    close(server_fd);
    return 0;
}