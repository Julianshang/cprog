/*************************************************************************
    > File Name: main.c
    > Author: Julian
    > Email:  1042029354@qq.com 
    > Created Time: Wed 02 Dec 2015 09:20:46 PM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int main()
{
    char *input, *output, *pos;

    input = "julianshang";
    output = "shan";

    pos = strstr(input, output);
    if(pos)
        printf("result: %s\n", pos);

    return 0;
}
