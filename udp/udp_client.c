#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#import <arpa/inet.h>

#define MAXLINE 4096
#define NDG 2000 /*datagrams to send*/
#define DGlen 1400/*length of each datagram*/

char* sock_ntop (const struct sockaddr *sa, socklen_t salen)
{
    char portstr[8];
    static char str[128];
    
    switch (sa->sa_family) {
        case AF_INET:
        {
            struct sockaddr_in *sin = (struct sockaddr_in *)sa;
            if (inet_ntop (AF_INET, &sin->sin_addr,
                           str, sizeof(str)) == NULL)
                return NULL;
            
            if (ntohs(sin->sin_port != 0))
            {
                snprintf (portstr, sizeof(portstr), ":%d",
                          ntohs (sin->sin_port));
                strcat (str, portstr);
            }
            
            return str;
        }
        default:
        {
            snprintf (str, sizeof(str), "sock_ntop: unknown AF_xxx %d, len %d", sa->sa_family, salen);
            return str;
        }
            
    }
    
    return (NULL);
}


char* Sock_ntop (const struct sockaddr *sa, socklen_t salen)
{
    char *ptr;
    
    if ((ptr = sock_ntop(sa, salen)) == NULL)
        perror ("sock_ntop error");
    
    return ptr;
}

void dg_cli(FILE *fp,int sockfd,const struct sockaddr * pservaddr,socklen_t servlen){
    
//    int n;
//    char sendline[MAXLINE],recvline[MAXLINE+1];
//    socklen_t len;
//    struct sockaddr *preply_addr;
//    preply_addr = malloc(servlen);
//    
//    while (fgets(sendline,MAXLINE,fp)!=NULL) {
//        sendto(sockfd,sendline,strlen(sendline),0,pservaddr,servlen);
//        printf("send mesg!\n");
//        
//        len = servlen;
//        n = recvfrom(sockfd,recvline,MAXLINE,0,preply_addr,&len-1);
//        /*用memcmp比较套接字地址结构本身*/
//        if(len != servlen || memcmp(pservaddr,preply_addr,len) != 0){
//            printf("reply from %s (ignored) \n",Sock_ntop(preply_addr,len));
//            fputs(recvline,stdout);
//            continue;
//        }
//        
//        recvline[n] = 0;/*null terminate*/
//        fputs(recvline,stdout);
//    }
    
    int n;
    char sendline[MAXLINE],recvline[MAXLINE+1];
    connect(sockfd,(struct sockaddr *)pservaddr,servlen);
    
    while (fgets(sendline,MAXLINE,fp)!=NULL) {
        write(sockfd,sendline,strlen(sendline));
        printf("send mesg!\n");
        
        n = read(sockfd,recvline,MAXLINE);
        recvline[n] = 0;
        fputs(recvline,stdout);
    }

}

int main(int argc,char **argv) {
    
    //对端地址
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;//IP4
    servaddr.sin_port = htons(9877);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    //servaddr.sin_addr.s_addr = htonl(0x7f000001); //or
    
    //建立连接
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (-1 == sockfd){
        perror("创建套接字失败! \n");
        exit(EXIT_FAILURE);
    }
    
   printf("套接字创建成功! \n");
   dg_cli(stdin,sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    
    
    return 0;
}


