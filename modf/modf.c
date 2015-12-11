/*************************************************************************
    > File Name: modf.c
    > Author: Julian
    > Email:  1042029354@qq.com 
    > Created Time: Thu 26 Nov 2015 11:43:34 PM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>

size_t strlcpy(char *dst, const char *src, size_t siz)
{
    size_t len = strlen(src);
    if (siz != 0) {
	    if (len >= siz) {
	        memcpy(dst, src, siz - 1);
	        dst[siz - 1] = '\0';
	    } else
	        memcpy(dst, src, len + 1);
    }
    return len;
}

// 获得带有7位小数的经纬度字符串表示方式
static char *deg_to_str_7f(double f)
{
    static char str[40];
    int negtive = 0;
    double abs_f, fdeg, fmin;
    long frac_deg;
    int deg, offset = 0;

    if(f < 0)
        negtive = 1;
    abs_f = fabs(f);

    if(abs_f < 0 || abs_f > 360){
        strlcpy(str, "NaN", sizeof(str));
        return str;
    }

    // type == deg_dd
    // DD.ddddddd
    fmin = modf(abs_f, &fdeg);
    deg = (int)fdeg;
    frac_deg = (long)(fmin * 10000000);
    
    if(negtive){
        str[0] = '-';
        offset = 1;
    }
    // 编码后的经纬度信息单位为1/10 micro degree，因此没有小数点
    snprintf(str + offset, sizeof(str) - offset, "%d%07ld",deg, frac_deg);
    return str;
}


static char *alt_to_str(double f)
{
    static char str[40];
    int offset;
   
    // wme使用的高度单位为0.1，而从gpsd读到的高度单位为m，
    // 因此进行转换。 
    long ftimes = (long)(f * 10);  

    // 1609.3中规定的有效范围
    if(ftimes > 61439)
        ftimes = 61439;
    if(ftimes < -4095)
        ftimes = -4095;

    if(ftimes < 0){
        str[0] = '-';
        offset = 1;
    } else {
        str[0] = '+';
        offset = 1;
    }
    // 编码后的经纬度信息单位为1/10 micro degree，因此没有小数点
    snprintf(str + offset, sizeof(str) - offset, "%d", ftimes);
    return str;
}

// 将误差散列到一个表上
int cfd_to_index(double epx, double epy)
{
    // 计算二维平面上的位置误差，误差最后都换算成以cm为单位
    long errhrzt;  // - cm
    double ephrzt; // - m
    
    ephrzt = sqrt(pow(epx, 2) + pow(epy, 2)); 
    errhrzt = ephrzt * 100;
     
    if(errhrzt < 2)             // <2cm
        return 15; /* 1 */ 
    else if(errhrzt < 5)        // [2cm, 5cm)    
        return 14; /* 2 */
    else if(errhrzt < 10)       // [5cm, 10cm)
        return 13;  /* 5 */
    else if(errhrzt < 20)       // [10cm, 20cm)
        return 12;  /* 10 */
    else if(errhrzt < 50)       // [20cm, 50cm)
        return 11;  /* 20 */    
    else if(errhrzt < 100)      // [50cm, 1m]
        return 10;  /* 50 */
    else if(errhrzt < 200)      // [1m, 2m)
        return 9;   /* 100 */
    else if(errhrzt < 500)      // [2m, 5m)
        return 8;   /* 200 */
    else if(errhrzt < 1000)     // [5m, 10m)
        return 7;   /* 500 */
    else if(errhrzt < 2000)     // [10m, 20m)
        return 6;   /* 1000 */
    else if(errhrzt < 5000)     // [20m, 50m)
        return 5;   /* 2000 */
    else if(errhrzt < 10000)    // [50m, 100m)
        return 4;   /* 5000 */
    else if(errhrzt < 20000)    // [100m, 200m)
        return 3;   /* 10000 */
    else if(errhrzt < 50000)    // [200m, 500m)
        return 2;   /* 20000 */
    else 
        return 1;   /* 50000 */ // >500m
}


int main()
{
    char *res;
    char *alt;
    int index;

    double f = -103.928211667;

    res = deg_to_str_7f(f);
    printf("res = %s\n", res);
    alt = alt_to_str(6144);
    printf("alt = %s\n", alt);
    index =cfd_to_index(0.3, 0.4);
    printf("index = %d\n", index);
    return 0;
}

