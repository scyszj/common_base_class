#include "stdio.h"
#include "string.h"
#include <sys/types.h>
#include <stdlib.h>

#include "comm_base.h"


comm_base::comm_base(void)
{
}

comm_base::~comm_base(void)
{
}
//是否为十六进制字符
static int is_ascii_hex(unsigned char data)
{
    if (data>='0' && data<='9') return data-'0';
    if (data>='A' && data<='F') return 10+data-'A';
    if (data>='a' && data<='f') return 10+data-'a';

    return -1;
}
//是否为十进字字符
static int is_ascii_dec(unsigned char data)
{
    if (data>='0' && data<='9') return data-'0';
    return -1;
}

//十六进制数转换为字符串
int comm_base::hex_to_hexstring(char hexdata[],unsigned int hexsize,char buff[],unsigned int buffsize)
{
    memset(buff,0,buffsize);
    if (buffsize<hexsize*5+1) return -1;

    unsigned int cnt=0;
    for (unsigned int i=0; i<hexsize; i++) {
        cnt+=sprintf((char *)&buff[cnt],"0x%02X ",(unsigned char)hexdata[i]);
    }

    buff[cnt]='\0';

    return cnt;
}
//十六进制字符串转换为十六进制数
int comm_base::hexstring_to_hex(char hexstring[],unsigned int hexsize,char buff[],unsigned int buffsize)
{
    unsigned int cnt=0;
    char *strbuff=(char *)malloc(hexsize+1);
    if((hexsize & 0x1)==1) {
        memcpy(strbuff+1,hexstring,hexsize);
        strbuff[0]='0';
        hexsize+=1;
    } else {
        memcpy(strbuff,hexstring,hexsize);
    }
    if(hexsize>4096*16) {
        return -1;
    }

    for (unsigned int i=0; i<hexsize; i++) {
        int h=0,l=0;

        if (is_ascii_hex(strbuff[i])<0) {
            continue;
        }

        if ((i+1)<hexsize) {
            if (strbuff[i+1]=='x' || strbuff[i+1]=='X') {
                i++;
                continue;
            }
        }

        if ((h=is_ascii_hex(strbuff[i]))<0) {
            continue;
        }

        i++;
        if(i<hexsize) {
            if((l=is_ascii_hex(strbuff[i]))<0) {
                if(cnt>=buffsize) {
                    free(strbuff);
                    return -1;
                }
                buff[cnt++]=h;
                continue;
            }
            if(cnt>=buffsize) {
                free(strbuff);
                return -1;
            }
            buff[cnt++]=(h<<4)+l;
        } else {
            buff[cnt++]=h;
        }
    }

    free(strbuff);
    return cnt;
}

//十进制字符串转换为BCD码
int comm_base::decstring_to_bcd(char hexstring[],unsigned int hexsize,char buff[],unsigned int buffsize)
{
    unsigned int cnt=0;
    char *strbuff=(char *)malloc(hexsize+1);
    if((hexsize & 0x1)==1) {
        memcpy(strbuff+1,hexstring,hexsize);
        strbuff[0]='0';
        hexsize+=1;
    } else {
        memcpy(strbuff,hexstring,hexsize);
    }

    if(hexsize>4096*16) {
        return -1;
    }

    for (unsigned int i=0; i<hexsize; i++) {
        int h=0,l=0;

        if (is_ascii_dec(strbuff[i])<0) {
            continue;
        }

        if ((i+1)<hexsize) {
            if (strbuff[i+1]=='x' || strbuff[i+1]=='X') {
                i++;
                continue;
            }
        }

        if ((h=is_ascii_dec(strbuff[i]))<0) {
            continue;
        }

        i++;
        if(i<hexsize) {
            if((l=is_ascii_dec(strbuff[i]))<0) {
                if(cnt>=buffsize) {
                    free(strbuff);
                    return -1;
                }
                buff[cnt++]=h;
                continue;
            }
            if(cnt>=buffsize) {
                free(strbuff);
                return -1;
            }
            buff[cnt++]=(h<<4)+l;
        } else {
            buff[cnt++]=h;
        }
    }

    free(strbuff);
    return cnt;
}

//计算16位CRC
unsigned short comm_base::get_crc(unsigned char data[],unsigned char data_count)
{
    unsigned short crc=0xF0;
    unsigned char i,m,n;

    for (i=0; i<data_count; i++) {
        n=data[i];
        crc ^= n;
        for (m = 0; m < 8; ++m) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = (crc >> 1);
            }
        }
    }

    return crc;
}

//等待指字毫秒数
int comm_base::msleep(unsigned int msecond)
{
    struct timeval t_timeval;

    t_timeval.tv_sec = 0;
    t_timeval.tv_usec = msecond * 1000;
    select(0, NULL, NULL, NULL, &t_timeval);

    return 0;
}

