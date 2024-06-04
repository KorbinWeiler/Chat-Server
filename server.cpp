#include "server.h"

#define MAX_SIZE 1024
#define MAX_CLIENTS 10
#define PORT 8080

int main(int argc, char const* argv[]){

    int server_fd, client_fds[MAX_CLIENTS];
    fd_set read_fds;
    char messageBuffer[MAX_SIZE];

    //Reset all clients
    for(int i = 0; i < MAX_CLIENTS; ++i){
        client_fds[i] = 0;
    }

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

    int addr_len = sizeof(address);
    int fd;
    int client_count;

    while(1){
        FD_ZERO(&read_fds);

        FD_SET(server_fd, &read_fds);
        int max = server_fd;

        for(int i = 0; i < MAX_CLIENTS; ++i){
            fd = client_fds[i];

            if(fd > 0){
                FD_SET(fd, &read_fds);
            }

            if(fd > client_count){
                client_count = fd;
            }
        }

        int activity = select(client_count + 1, &read_fds, NULL, NULL, NULL);
        int new_socket;
        if(FD_ISSET(server_fd, &read_fds)){
            new_socket = accept(server_fd, (struct sockaddr*)&address, 
            (socklen_t*)&addr_len);
        }

        for(int i = 0; i < MAX_CLIENTS; ++i){
            if(client_fds[i] == 0){
                client_fds[i] = new_socket;
                break;
            }
        }

        for(int i = 0; i < MAX_CLIENTS; ++i){

            int msg_read;
            fd = client_fds[i];

            if(FD_ISSET(fd, &read_fds)){

                if(msg_read = read(fd, messageBuffer, MAX_SIZE)){
                    
                }
            }    
        }
    }

    close(server_fd);
    return 0;
}