/***********************************************************************
 * File : protocal.c
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2015-10-07     ginozh   1.0    created
 * 
 ***********************************************************************
 */
#include "protocal.h"
#include <string.h>
#include <arpa/inet.h>

#define    NETWORK2HOST_32(v)        (uint32_t)(ntohl(v))
#define    GET_8_CONV(s, l)    do {(l) = *((uint8_t *)(s)); (s) += sizeof(uint8_t); } while (0)
#define    GET_32_CONV(s, l)    do {(l) = NETWORK2HOST_32(*((uint32_t *)(s))); (s) += sizeof(uint32_t);  } while (0)

int pkg_pack(const uint8_t *body, const int32_t body_len
        , const uint8_t cmd, uint8_t *pkg_bufp, const int32_t pkg_buf_len)
{
    int32_t len;
    uint8_t *pch;
    int32_t *puiDataLen;
    if(!body || !pkg_bufp ){ 
        return -1; 
    } 
    len = pkg_buf_len;  
    if(len<=0)  
    {  
        return -2;  
    }  
    if (len <= (int32_t)(sizeof(tSrvHeader)+body_len+1))  
        return -3;  
    /*start*/  
    len -= (int32_t)sizeof(uint8_t);  
    pch=pkg_bufp;  
    *pch++=FLAG_START;  
    /*len*/  
    len -= (int32_t)sizeof(uint32_t);  
    puiDataLen=(int32_t *)pch;  
    pch+=sizeof(uint32_t);  
    /*cmd*/  
    len -= (int32_t)sizeof(uint8_t);  
    *pch++=cmd;  
    /*body*/
    if (len < body_len+1)  
        return -4;  
    memcpy(pch, body, body_len);
    pch+=body_len;  
    len-=body_len;  
    /*end*/ 
    len -= (int32_t)sizeof(uint8_t);  
    *pch++=FLAG_END;;  
    *puiDataLen=htonl(pch-pkg_bufp);  
    return pch-pkg_bufp;  
}
int pkg_unpack_header(const uint8_t *pkg_bufp, const int32_t pkg_buf_len, uint8_t* cmd)
{
    const uint8_t *pData = pkg_bufp;
    const uint8_t *pBufEnd = pkg_bufp + pkg_buf_len - 1; 
    int i;

    if (NULL == pkg_bufp || pBufEnd <= pkg_bufp){
        return -1;
    }    
    if (FLAG_START != *pData++ || FLAG_END != *pBufEnd){
        return -2;
    }    
    GET_32_CONV(pData, i);
    if (i != pkg_buf_len){
        return -3;
    }
    int iCmd;
    GET_8_CONV(pData, iCmd);
    if(cmd){
        *cmd=iCmd;
    }
    //int32_t iLenBody=pkg_buf_len-(int32_t)(pData-pkg_bufp)-1;
    return (int32_t)(pData-pkg_bufp);
}
int pkg_unpack(const uint8_t *pkg_bufp, const int32_t pkg_buf_len, char *body, int32_t body_len)
{
    int32_t iLenBody;
    const uint8_t *pData = pkg_bufp;  
    const uint8_t *pBufEnd = pkg_bufp + pkg_buf_len - 1;  
    int i;  

    if (!pkg_bufp || pBufEnd <= pkg_bufp){
        return -1;  
    }  
    /* verify */  
    if (FLAG_START != *pData++ || FLAG_END != *pBufEnd){  
        return -2;  
    }  
    /*size */
    GET_32_CONV(pData, i);  
    if (i != pkg_buf_len){  
        return -3;  
    }  
    /*cmd*/
    pData++;  
    /*body*/
    iLenBody=pkg_buf_len-(int32_t)(pData-pkg_bufp)-1;  
    if(iLenBody>=body_len)
    {
        return -4;
    }
    memcpy(body, pData, iLenBody);
    //body_len=iLenBody;
    
    return body_len;
}
