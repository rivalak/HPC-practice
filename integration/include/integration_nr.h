/**
 * 用一个双向链表 interval_t 来存放区间集合和被积函数值
*/

typedef struct INTERVAL_T {
    double x0, f0, x1, f1, fc;
    struct INTERVAL_T *last, *next;
} interval_t;

double integration_nr(double a, double b, double eps, void(*F1)(double x, double *f));
