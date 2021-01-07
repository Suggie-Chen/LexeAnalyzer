#pragma once
#include<iostream>

const int MAXBUFLEN = 2048;
/*读入缓冲区相关*/
class Buffer {
public:
    Buffer() {
        lhead = 0;
        lend = MAXBUFLEN / 2 - 1;
        rhead = MAXBUFLEN / 2;
        rend = MAXBUFLEN - 1;
        //buffer[MAXBUFLEN] = { 0 };
        memset(buffer, 0, MAXBUFLEN);
        buffer[lend] = EOF;
        buffer[rend] = EOF;
        pbegin = 0;
        pforward = 0;
    }
    void beginRead();
    void retract();
    void get_char();
    void get_nbc();

private:
    int forwarding();
    char buffer[MAXBUFLEN];
    int lend;
    int rend;
    int lhead;
    int rhead;
    int pbegin;
    int pforward;
};