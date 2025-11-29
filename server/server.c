#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8273
#define SIZE 1024

void HandleClient(int new_fd){
    char filename[SIZE];

    read(new_fd, filename, SIZE);
    printf("Client requested file: %s\n", filename);
    FILE *f = fopen(filename, "rb");

    if (!f) {
    send(new_fd, "NOTFOUND", 8, 0);
    perror("[-] File Not found");
    close(new_fd);
    return;       
    }
    
    char buffer[SIZE];
    int bytes;
    while ((bytes = fread(buffer, 1, SIZE, f)) > 0) {
        int x = send(new_fd, buffer, bytes, 0);
        if(x == -1){
            printf("[-] Error of send\n");
            break;
        }
    }

    printf("File sended\n");
    fclose(f);
    close(new_fd);


}

int main(){

    int sockfd, new_fd;
    struct sockaddr_in my_addr, client_addr;
    socklen_t sin_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("[-] Socket error");
        exit(1);
    }

    printf("[+] Socket has been created\n");

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

    int x = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
    if(x == -1){
        perror("[-] Bind error");
        exit(1);
    }

    printf("[+] Socket has been binded to port: %d\n",PORT);

    x = listen(sockfd, 5);
    if(x == -1){
        perror("[-] Error of listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1){
        
        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

        if(new_fd < 0){
            perror("[-] Error of accept");
            continue;
        }

        printf("New Client connected\n");

        int pid = fork();
        if(pid == 0){
            close(sockfd);
            HandleClient(new_fd);
            exit(0);
        }
        else if (pid > 0) 
        {
            close(new_fd);
        }
        else {
            perror("[-] Error of forking\n");
        }    
    }
    
    close(sockfd);
    return 0;
    


}