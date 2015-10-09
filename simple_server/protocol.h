/***********************************************************************
 * File : protocal.h
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2015-10-07     ginozh   1.0    created
 * 
 ***********************************************************************
 */

#ifndef  PROTOCAL_INC
#define  PROTOCAL_INC

#include    <stdint.h>

#pragma pack(1)
enum {
    FLAG_START =0x04, 
    FLAG_END= 0x05,
};

typedef    struct {
    uint8_t        ucStart;    
    uint32_t        uPkgLen;    
    uint8_t        ucCmd;    
}tSrvHeader;
#pragma pack()
int pkg_pack(const uint8_t *body, const int32_t body_len
        , const uint8_t cmd, uint8_t *pkg_bufp, const int32_t pkg_buf_len);
int pkg_unpack_header(const uint8_t *pkg_bufp, const int32_t pkg_buf_len, uint8_t* cmd);
int pkg_unpack(const uint8_t *pkg_bufp, const int32_t pkg_buf_len, char *body, int32_t body_len);

#endif   /* ----- #ifndef PROTOCAL_INC  ----- */

