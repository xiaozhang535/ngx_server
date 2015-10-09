/***********************************************************************
 * File : tcp_server.c
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2015-10-05     ginozh   1.0    created
 * 
 ***********************************************************************
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "protocol.h"

int main ( int argc, char *argv[] )
{
    int listenfd,connfd,n;
    int iInDataLen;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t clilen;
    char mesg[1000];
    char szIp[] = "127.0.0.1";
    uint16_t usPort = 32000;
    int32_t iPkgLen;
    int32_t iBodyIdx;
    tSrvHeader *psrvHead;

    listenfd=socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(szIp);//htonl(INADDR_ANY);
    servaddr.sin_port=htons(usPort);
    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

    listen(listenfd,1024);

    for(;;)
    {
        clilen=sizeof(cliaddr);
        connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
        iInDataLen=0;
        iPkgLen=0;
        while(1)
        {
            n = recvfrom(connfd,mesg+iInDataLen,sizeof(mesg)-iInDataLen,0,(struct sockaddr *)&cliaddr,&clilen);
            if(n==0)
            {
                printf("client close the socket\n");
                close(connfd);
                break;
            }
            else if(n<0)
            {
                printf("errno: %d errstr: %s\n", errno, strerror(errno));
                close(connfd);
                break;
            }
            printf("get len: %d\n", n);
            iInDataLen+=n;
            //包头
            if(iPkgLen==0)
            {
                if(iInDataLen<sizeof(tSrvHeader))
                {
                    continue;
                }
                psrvHead=(tSrvHeader*)mesg;
                if(psrvHead->ucStart == FLAG_START)
                {
                    iPkgLen=ntohl(psrvHead->uPkgLen);
                    if(iPkgLen > sizeof(mesg)){
                        printf("max buff size\n");
                        close(connfd);
                        break;
                    }
                }
                else
                {
                    printf("error start flag\n");
                }
            }
            if(iInDataLen<iPkgLen)
            {
                continue;
            }
            iBodyIdx=pkg_unpack_header((const uint8_t*)mesg, iPkgLen, 0);
            if(iBodyIdx>0)
            {
                mesg[iPkgLen-1]=0;
                printf("body: %s\n",mesg+iBodyIdx);
            }
            iInDataLen -= iPkgLen;
            if(iInDataLen == 0)
            {
                continue;
            }
            memmove(mesg, mesg+iPkgLen, iInDataLen);
            iPkgLen=0;
        }
        close(connfd);
    }
}
