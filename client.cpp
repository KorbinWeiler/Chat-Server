#include "client.h"

#define MAX_SIZE 1024

int main(int argc, const char* argv[]){

    srand((unsigned)time(0));

    char* username = (char*)"";

    //segmentation error somewhere in here
    if(argc >= 2){
        //username = argv[2];
        //strcpy(username, argv[2]);
        username = (char*)argv[2];
    }
    else{ //seg fault
        char* user = (char*)"User";
        int userNumber = rand() % 20000;
        char temp[10];
        sprintf(temp, "%d", userNumber);
        strcat(user, temp);
        username = user;
    }
    char tempUser[MAX_SIZE];
    strcpy(tempUser, username);

    int client_fd;
    
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket failure");
        exit(0);
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if((connect(client_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) < 0){
        std::cout << "connection failed" << std::endl;
        exit(0);
    }

    while(true){
        bool userMessageSent = false;

        if(fork() == 0){
            while(!userMessageSent){

                char buffer[MAX_SIZE] = {0};
                
                if(read(client_fd, buffer, sizeof(buffer) - 1) != 0){
                    if(buffer != ""){
                        std::cout << buffer << std::endl;
                    }
                }
                memset(buffer, NULL, sizeof(buffer));

            }
            exit(0);
        }

        else{
            char* userMessage;
            std::cin >> userMessage;

            if(userMessage != "" && userMessage != "exit"){
                char* message = username;
                strcat(message, ": ");
                strcat(message, userMessage);
                send(client_fd, (char*)message, strlen(message), 0);
                strncpy(message, tempUser, sizeof(tempUser));
            }

            else if(userMessage == "exit"){
                break;
            }
        }
    }

    std::cout << username << " has disconnected" << std::endl;
    close(client_fd);
}