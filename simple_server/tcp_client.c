/***********************************************************************
 * File : tcp_client.c
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
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>

#include "protocol.h"

int main ( int argc, char *argv[] )
{
    char szRemoteIp[]="127.0.0.1";
    uint16_t usRemotePort=32000;
    uint8_t szSndBuf[1024];
    int32_t iSndLen=sizeof(szSndBuf);
    uint8_t szBody[]="test";
    uint32_t uLenLeft;
    uint32_t uLenSend;
    uint32_t uLenOneMsg;
    //uint32_t ulTimeout=1000; //ms

    struct sockaddr_in m_astTcpSvrAddr; 
    int iSocket = -1;
    int iFlags = 1;
    //struct timeval tmTimeout;
    //uint8_t szRcvBuf[1024]={0};
    //int32_t usRecvBufLen=sizeof(szRcvBuf);
    int iRcvBufLen = 8 * 1024;
    int i;

    //signal(SIGPIPE, SIG_IGN);

    if((iSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
    {
        return -1 ;
    }
    if(setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, &iFlags, sizeof(iFlags)) == -1)
    {
        close(iSocket);
        return -1;
    }
    if(setsockopt(iSocket, SOL_SOCKET, SO_RCVBUF, (char*)&iRcvBufLen, sizeof(iRcvBufLen)) == -1)
    {
        close(iSocket);
        return -1 ;
    }
    bzero(&m_astTcpSvrAddr, sizeof(m_astTcpSvrAddr));
    m_astTcpSvrAddr.sin_family = AF_INET;
    m_astTcpSvrAddr.sin_addr.s_addr = inet_addr(szRemoteIp);
    m_astTcpSvrAddr.sin_port = htons(usRemotePort);
    if((connect(iSocket,(struct sockaddr *)&m_astTcpSvrAddr,sizeof(m_astTcpSvrAddr)))<0)
    {
        close(iSocket);
        return -1 ;
    }
    uLenOneMsg=pkg_pack( szBody, strlen((const char*)szBody),0, szSndBuf, iSndLen);
    if(uLenOneMsg<0)
    {
        return -1;
    }
    iSndLen=uLenOneMsg;
    for(i=0; i<=2; i++)
    {
        memcpy(szSndBuf+iSndLen, szSndBuf, uLenOneMsg);
        iSndLen+=uLenOneMsg;
    }
    uLenLeft = iSndLen ;
    uLenSend = 0;
    while(uLenLeft>0){
        int byte_send = send(iSocket, szSndBuf + uLenSend, 2, 0);
        if(byte_send<=0){
            printf("errno: %d errstr: %s\n", errno, strerror(errno));
            close(iSocket);
            return -1 ;
        }
        uLenSend+=byte_send;
        uLenLeft-=byte_send;
        sleep(1);
    }
    printf("send one\n");

    //1,封装数据
    //2,创建、连接端口
    //3,发送数据
    //4,接收数据
    return 0;
}
