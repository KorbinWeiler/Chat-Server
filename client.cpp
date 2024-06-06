#include "client.h"

#define MAX_SIZE 1024

int main(int argc, const char* argv[]){

    srand((unsigned)time(0));

    char* username;

    if(argc <= 1){
        //username = argv[2];
        strcpy(username, argv[2]);
    }
    else{
        char* user = "User";
        int userNumber = rand() % 20000;
        char temp[10];
        sprintf(temp, "%d", userNumber);
        strcat(user, temp);
        username = user;
    }

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(client_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    while(true){
        bool userMessageSent = false;

        if(fork() == 0){
            while(!userMessageSent){

                char buffer[MAX_SIZE];
                read(client_fd, buffer, sizeof(buffer) - 1);
                std::cout << buffer << std::endl;

            }
            exit(0);
        }

        else{
            std::string userMessage;
            std::cin >> userMessage;

            if(userMessage != "" && userMessage != "exit"){
                char* message = username;
                strcat(message, ": ");
                send(client_fd, (char*)message, strlen(message), 0);
            }

            else if(userMessage == "exit"){
                break;
            }
        }
    }

    std::cout << username << " has disconnected" << std::endl;
    close(client_fd);
}