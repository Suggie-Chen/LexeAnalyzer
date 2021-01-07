//删除c语言程序中所有的注释语句，要正确处理带引号的字符串与字符串常量
#include"removeComments.h"

void in_comment(FILE* fpin, FILE* fpout) {
    char c, d;
    c = getc(fpin);
    d = getc(fpin);
    while (c != '*' || d != '/')
    {
        c = d;
        d = getc(fpin);
    }
}

void deleteTail(FILE* fpin, FILE* fpout) {
    char c;
    c = getc(fpin);
    while (c != '\n')
    {
        c = getc(fpin);
    }
    putc('\n', fpout);
}

void rcomment(char c, FILE* fpin, FILE* fpout) {

    if (c == '"') {
        putc(c, fpout);
        char d;
        d = getc(fpin);
        while (d != '"') {
            putc(d, fpout);
            d = getc(fpin);
        }
        putc(d, fpout);
    }
    else if (c == '/') {
        char d;
        d = getc(fpin);
        if (d == '*')
            in_comment(fpin, fpout);
        else if (d == '/')
            deleteTail(fpin, fpout);
        else
        {
            putc(c, fpout);
            putc(d, fpout);
        }
    }
    else
        putc(c, fpout);
}

int removeComments(FILE* fpin, FILE* fpout)
{
    int flag1 = fopen_s(&fpin, "C:\\Users\\92994\\Desktop\\test.cpp", "r");
    int flag2 = fopen_s(&fpout, "C:\\Users\\92994\\Desktop\\out.txt", "w");
    if ((flag1 == 0) && (flag2 == 0))
        printf("Files have been opened successfully.");
    else
        printf("Failed to open the files!");

    char c;
    while ((c = getc(fpin)) != EOF)
    {
        rcomment(c, fpin, fpout);
    }
        
    fclose(fpin);
    fclose(fpout);
    return 0;
}