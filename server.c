#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0 );
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);



    int bytes_sent, bytes_received; 
    char buffer[1024]; 




    if (server_fd == -1){
        perror ("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port =htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("bind");
        return 1;
    }   
    if (listen(server_fd,5 ) ==-1){
        perror("listen");
        return -1;
    }
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd == -1){
        perror("accept");
        return 1;
    }
    printf("Client connected!\n");
    while(1){
        bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes_received<= 0){
            printf("Client disconnected\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Client said: %s\n", buffer);
        send(client_fd, buffer, bytes_received, 0);

    }
    close(client_fd);
    close(server_fd);

    return 0;


    
}