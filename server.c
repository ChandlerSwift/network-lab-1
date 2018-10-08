#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <file to send> \n",argv[0]);
        return 1;
    }

    char* filename = argv[1];

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    
    char* source = malloc(sizeof(char)*10000); // todo: dynamic based on file size
    FILE *fp = fopen(filename, "r");
    size_t newLen;
    if (fp != NULL) {
        newLen = fread(source, sizeof(char), 10000, fp);
        if ( ferror( fp ) != 0 ) {
            fputs("Error reading file", stderr);
        } else {
            source[newLen++] = '\0'; /* Just to be safe. */
        }
        fclose(fp);
    } else {
        return 2;
    }
    
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        write(connfd, source, newLen); 

        close(connfd);
        sleep(1);
     }
}
