#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int evaluation_count = 0;

/**
*被积函数
*/
double F(double x) {
    size_t i;
    //为增加计算时间引入的空循环
    for (i = 0; i < 5000000; ++i) {
        
    }
    evaluation_count++;

    // f(x) = 4 / (1 + x^2)
    return 4.0 / (1.0 + x * x);
}

/**
*积分精确值 （用于检查结果）
*/
double RESULT = 3.141592653589793;

/**
*返回当前CPU和墙上时间
*/
void gettime(double *cpu, double *wall) {
    struct timeval tv;
    struct rusage ru;
    if (cpu != NULL) {
        getrusage(RUSAGE_SELF, &ru);
        *cpu = ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec * 1e-6;
    }
    if (wall != NULL) {
        gettimeofday(&tv, NULL);
        *wall = tv.tv_sec + (double)tv.tv_usec * 1e-6;
    }
}
