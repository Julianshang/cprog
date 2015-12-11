#
/*************************************************************************
    > File Name: main.c
    > Author: Julian
    > Email:  1042029354@qq.com 
    > Created Time: Tue 01 Dec 2015 04:53:44 PM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

typedef struct log{
    char logtime[20];
    FILE *logfile;
}LOG;

LOG loging;

static void settime(){
    time_t timer=time(NULL);
    
    strftime(loging.logtime,20,"%Y %m %d %H:%M:%S",localtime(&timer));
}

#define PRINT(fmt, args...) do{\
    settime(); \
    fprintf(stderr, "[%s] rtl3d: ", loging.logtime); \
    fprintf(stderr, fmt, ##args); \
} while(0)

    /*
    va_list ap; \
    va_start(ap, fmt); \
    printf(fmt, ap); \
    va_end(ap); \
    */
int main()
{
    char *str = "test";

    while(1){
        PRINT("%s\n", str);
        sleep(1);
    }
    return 0;
}
