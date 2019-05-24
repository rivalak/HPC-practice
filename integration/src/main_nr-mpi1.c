/* 非递归方式自适应数值积分串行主程序: 非阻塞通信 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "integration_nr.h"
#include "function.h"

static size_t count = 0;
static int nprocs, myrank;

static void F1(double x, double *f) /* 该函数只在 Master 中调用 */
{
    static MPI_Request *req = NULL;
    static MPI_Status *sta = NULL;
    static double *xsave = NULL;
    static int size = 0, n = 0, slave;

    if (f == NULL) {
        if (n) {
            MPI_Waitall(n + n, req, sta);
            n = 0;
        }
    }
    else {
        if (n >= size) {
            req = realloc(req, 2 * (size += 128) * sizeof(*req));
            sta = realloc(sta, 2 * size * sizeof(*sta));
            xsave = realloc(xsave, size * sizeof(*xsave));
        }
        slave = count % (nprocs - 1) + 1;
        xsave[n] = x;
        MPI_Isend(xsave + n, 1, MPI_DOUBLE, slave, 1, MPI_COMM_WORLD, req+n+n);
        MPI_Irecv(f, 1, MPI_DOUBLE, slave, 1, MPI_COMM_WORLD, req+n+n+1);
        n++;
        count++;
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s epsilon\n", argv[0]);
        fprintf(stderr, "Example: %s 1e-4\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (nprocs < 2) {
        fprintf(stderr, "This program needs at least two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (myrank == 0) { /* Master */
        double res, wall0, wall1;
        int i;
        gettime(NULL, &wall0);
        res = integration_nr(0.0, 1.0, atof(argv[1]), F1);
        gettime(NULL, &wall1);
        printf("result=%0.16lf, error=%0.4le, wtime=%0.4lf\n",
        res, res - RESULT, wall1 - wall0);
        printf("%u function evaluations.\n", count);

        /* Tell the slaves to quit */
        for (i = 1; i < nprocs; i++)
        MPI_Send(&res, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    }
    else { /* Slave */
        while (1) {
            double d;
            MPI_Status sta;
            MPI_Recv(&d, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &sta);
            if (sta.MPI_TAG == 0) break;
            d = F(d);
            MPI_Send(&d, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
