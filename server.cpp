

//#include <iostream>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>

#define BUF_SIZE 1024
#define MAX 10000

int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    int sfd;
    char buf[BUF_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    getaddrinfo(NULL, argv[1], &hints, &result);

    
    for(rp=result; rp!=NULL; rp=rp->ai_next){
        sfd=socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1) continue;
        if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) break;
        close(sfd);
    }
    printf("123\n\n");
    freeaddrinfo(result);


    int newsockfd;
    socklen_t client;
    struct sockaddr_storage client_addr;
    listen(sfd,5);

    client=sizeof(client_addr);
    newsockfd=accept(sfd,(struct sockaddr *)&client_addr, &client);
    
    //if(newsockfd<0) perror("error");
  
    //read(newsockfd, buf, BUF_SIZE);
    srand(unsigned(time(0)));
    int r_port = 2014 + rand()%MAX;
    sprintf(buf,"%d",r_port);
    printf("Negotiation detected. Selected Random Port: %d\n",r_port);

    close(sfd);
    write(newsockfd, buf, sizeof(buf));
    close(newsockfd);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_DGRAM;
    hints.ai_flags=AI_PASSIVE;
    hints.ai_protocol=0;


    getaddrinfo(NULL, buf, &hints, &result);

    for(rp=result; rp!=NULL; rp=rp->ai_next){
        sfd=socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1) continue;
        if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) break;
        close(sfd);
    }

    freeaddrinfo(result);

    FILE *fp;
    fp=fopen("receive.txt", "w");

    char end_point[5]= "end";

    for(;;){
        peer_addr_len=sizeof(struct sockaddr_storage);

        nread=recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr*) &peer_addr, &peer_addr_len);
        
        if(nread == -1) continue;

        char host[NI_MAXHOST], service[NI_MAXSERV];
        getnameinfo((struct sockaddr*) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        //test
        

        if(strcmp(end_point, buf) == 0){
            sendto(sfd, buf, nread, 0, (struct sockaddr*) &peer_addr, peer_addr_len);
            break;
        }
        else{
            fprintf(fp, "%s", buf );
              
        }
        sendto(sfd, buf, nread, 0, (struct sockaddr*) &peer_addr, peer_addr_len);
    }
    close(sfd);
    fclose(fp);

    exit(EXIT_SUCCESS);

}




