#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>
#define BUF_SIZE 1024
#define MAX 10000

int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    //struct socketaddr_storage peer_addr;
    //socklen_t, peer_addr_len;
    ssize_t nread;
    int sfd, s, word_num;
    char buf[BUF_SIZE];


    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM; //use STREAM type
    hints.ai_flags= AI_PASSIVE;

    getaddrinfo(argv[1], argv[2], &hints, &result);
    for(rp=result; rp != NULL; rp=rp->ai_next){
        sfd=socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1) continue;
        if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) break;
        close(sfd);
    }

    if(rp == NULL) {
        printf("error");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    //int newsockfd;
    //write(sfd, buf, 117);
    nread=read(sfd, buf, BUF_SIZE);
    close(sfd);
    printf("Random port: %s\n", buf);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM; //use udgram type
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    getaddrinfo(argv[1], buf, &hints, &result);

    for(rp=result; rp!=NULL; rp=rp->ai_next){
        sfd=socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1) continue;
        if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) break;
        close(sfd);
    }

    if(rp == NULL) printf("error2\n");

    freeaddrinfo(result);

    FILE *fp;
    fp=fopen(argv[3],"r");
    char str[BUF_SIZE];
    size_t strLen;
    //word_num=fread(str, 1, 4, fp);
    while((word_num=fread(str, 1, 4, fp)) != 0){
        strLen=word_num+1;
        str[strLen-1] = '\0';

        write(sfd, str, strLen);
        nread=read(sfd, buf, BUF_SIZE);
        for(int i =0; i<nread; i++){
                buf[i] = toupper(buf[i]);
            }
        printf("%s\n", buf);
    }
    fclose(fp);
    
    char end_point[5]="end";
    strLen=strlen(end_point)+1;
    write(sfd, end_point, strLen);
    nread=read(sfd, buf, BUF_SIZE);
	
    close(sfd);
    exit(EXIT_SUCCESS);


}