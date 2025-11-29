#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/socket.h>

#define PORT 8273
#define SIZE 1024

int main(){
    int sockfd;
    struct sockaddr_in server_addr; 
    char filename[SIZE];

    while (1)
    {
       int choix; 
       printf("-------Menu--------\n");
       printf("1.Download a file\n");
       printf("2.quit\n");
       scanf("%d",&choix);
       if (choix == 2)
       {
            printf("Goodbye\n");
            break;
       }

       printf("Enter filename: ");
       scanf("%s",filename);
       
       sockfd = socket(AF_INET, SOCK_STREAM, 0);
       server_addr.sin_family = AF_INET;
       server_addr.sin_port = htons(PORT);
       server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
       bzero(&(server_addr.sin_zero), 8);

       connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

       send(sockfd, filename, strlen(filename)+1, 0);


       char buffer[SIZE];
       int bytes = recv(sockfd, buffer, SIZE, 0); 
       if (strncmp(buffer, "NOTFOUND", 8) == 0) { // If file not found
            printf("File Not found");  
            close(sockfd);
            continue; 
        }
        else {
            FILE *f = fopen(filename, "wb");
            fwrite(buffer, 1, bytes, f);
            while ((bytes = recv(sockfd, buffer, SIZE, 0)) > 0) {
                fwrite(buffer, 1, bytes, f);
            }

            printf("File recieved\n");
            fclose(f);

        }   

    }

    close(sockfd);
    return 0;


}