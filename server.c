#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>


void getCurrTime(char *buf, size_t size){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void log_event(FILE *logfile, const char *message){
    char timebuf[64];
    getCurrTime(timebuf, sizeof(timebuf));
    printf("[%s] %s\n", timebuf, message);
    fprintf(logfile, "[%s] %s\n", timebuf, message);
    fflush(logfile);
    
}

int main(int argc, char *argv[]){
    char logmsg[1100];
    char client_ip[INET_ADDRSTRLEN];


    FILE *logfile = fopen("server.log", "a");
    if (logfile == NULL){
        perror("fopen");
        return 1;
}
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0 );
    log_event(logfile, "SERVER TURNING ON");
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
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, sizeof(client_ip));
    snprintf(logmsg, sizeof(logmsg), "CLIENT CONNECTED: %s", client_ip);
    log_event(logfile, logmsg);
    while(1){
        bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);    
        if (bytes_received<= 0){
            printf("Client disconnected\n");
            log_event(logfile, "CLIENT DISCONNECTED");
            break;
        }
        buffer[bytes_received] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        snprintf(logmsg, sizeof(logmsg), "MESSAGE: %s", buffer);
        printf("Client said: %s\n", buffer);
        
        log_event(logfile, logmsg);
        send(client_fd, buffer, bytes_received, 0);

    }
    close(client_fd);
    log_event(logfile, "SERVER SHUTTING DOWN");
    close(server_fd);

    return 0;


    
}