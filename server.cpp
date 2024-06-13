#include "server.h"

#define MAX_SIZE 1024
#define MAX_CLIENTS 10
#define PORT 8080

int main(int argc, char const* argv[]){

    int server_fd, client_fds[MAX_CLIENTS];
    fd_set read_fds;
    int opt = 1;
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

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
            &opt, sizeof(opt))){
        perror("setsocket");
        exit(0);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));
    
    //Bind the server socket
    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address))){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }
    else{
        std::cout << "Server Starting..." << std::endl;
        std::cout << "Server listening on port " << argv[1] << std::endl;
    }

    if(listen(server_fd, 10) < 0);

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

                if(msg_read = read(fd, messageBuffer, MAX_SIZE) == 0){
                    getpeername(fd, (struct sockaddr*)&address, (socklen_t*)&address);
                    close(fd);
                    client_fds[i] = 0;
                }
                else{
                    messageBuffer[msg_read] = '\0';
                    std::cout << messageBuffer << std::endl;
                    send(fd, messageBuffer, sizeof(messageBuffer), 0);
                }
            }    
        }
    }

    close(server_fd);
    return 0;
}