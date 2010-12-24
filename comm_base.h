#ifndef COMM_BASE_H_INCLUDED
#define COMM_BASE_H_INCLUDED

class comm_base {
public:
    comm_base(void);
    ~comm_base(void);

    int hex_to_hexstring(char hexdata[],unsigned int hexsize,char buff[],unsigned int buffsize);
    int hexstring_to_hex(char hexstring[],unsigned int hexsize,char buff[],unsigned int buffsize);
    int decstring_to_bcd(char hexstring[],unsigned int hexsize,char buff[],unsigned int buffsize);

    unsigned short get_crc(unsigned char data[],unsigned char data_count);
    int msleep(unsigned int msecond);
};



#endif // COMM_BASE_H_INCLUDED
