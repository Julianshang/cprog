/*************************************************************************
    > File Name: main.c
    > Author: Julian
    > Email:  1042029354@qq.com 
    > Created Time: Fri 11 Dec 2015 03:20:53 PM CST
    > 说明:为了演示当用户输入中包含了格式化字符串时可能导致的安全问题
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdio.h>

int main()
{
    char input[1000];

    char random[1000];
     
    if (fgets(input, sizeof(input) - 1, stdin) == NULL){
        // handle error
    }

    input[sizeof(input) - 1] = '\0';
    printf(input);

    return 0;
}

