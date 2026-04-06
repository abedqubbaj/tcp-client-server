#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


int main(int argc, char *argv[]){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0 );
    struct sockaddr_in server_addr;

    char *connected_msg = "Client has connected";
    int msg_len = strlen(connected_msg);
    int bytes_sent, bytes_received; 
    char buffer[1024]; 


    if (client_fd == -1){
        perror ("socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port =htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));


        if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("connect");
        return 1;
    }


    while(1){
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (strncmp(buffer, "quit\n", 5) == 0){
                break;
            }
            send(client_fd, buffer, strlen(buffer), 0);
            bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
            buffer[bytes_received] = '\0';
            printf("Server said: %s\n", buffer);

        }


    }
    close(client_fd);


    return 0;


    
}