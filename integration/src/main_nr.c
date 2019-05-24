#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "integration_nr.h"
#include "function.h"

static void F1(double x, double *f) {
    if (f == NULL) {
        return; //串行程序不做处理
    }
    *f = F(x);
}

int main(int argc, char *argv[])
{
    double a = 0.0, b = 1.0;
    double res, cpu0, cpu1, wall0, wall1;
    if (argc != 2) {
        fprintf(stderr, "Usage:    %s epsilon\n", argv[0]);
        fprintf(stderr, "Example: %s 1e-4\n", argv[0]);
        return 1;
    }

    gettime(&cpu0, &wall0);
    res = integration_nr(a, b, atof(argv[1]), F1);
    gettime(&cpu1, &wall1);
    printf("result=%0.16lf, error=%0.4le, cputime=%0.4lf, wtime=%0.4lf\n",
    res, res - RESULT, cpu1 - cpu0, wall1 - wall0);
    printf("%u function evaluations.\n", evaluation_count);
    return 0;

}
