/* 非递归方式自适应数值积分串行主程序: Scatter/Gather方式 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "integration_nr.h"
#include "function.h"
static size_t count = 0;
static int nprocs, myrank;
static int *cnts = NULL, *displs = NULL;

static void
F1(double x, double *f) /* 该函数只在 Master 中调用 */
{
static double *xsave = NULL, **fsave = NULL;
static int size = 0, n = 0;

if (f == NULL) {
int i;
if (cnts == NULL) cnts = realloc(cnts, nprocs * sizeof(*cnts));
if (displs == NULL) displs = realloc(displs, nprocs * sizeof(*displs));
cnts[0] = 0;
displs[0] = 0;
for (i = 1; i < nprocs; i++) {
cnts[i] = n / (nprocs - 1) + (i <= n % (nprocs - 1) ? 1 : 0);
displs[i] = displs[i - 1] + cnts[i - 1];
}
MPI_Scatter(cnts, 1, MPI_INT, &i, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Scatterv(xsave, cnts, displs, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0,
MPI_COMM_WORLD);
MPI_Gatherv(NULL, 0, MPI_DOUBLE, xsave, cnts, displs, MPI_DOUBLE, 0,
MPI_COMM_WORLD);
for (i = 0; i < n; i++) *(fsave[i]) = xsave[i];
count += n;
n = 0;
}
else {
if (n >= size) {
xsave = realloc(xsave, (size += 128) * sizeof(*xsave));
fsave = realloc(fsave, size * sizeof(*fsave));
}
xsave[n] = x;
fsave[n++] = f;
}
}

int
main(int argc, char **argv)
{
int i, n;

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
gettime(NULL, &wall0);
res = integration_nr(0.0, 1.0, atof(argv[1]), F1);
gettime(NULL, &wall1);
printf("result=%0.16lf, error=%0.4le, wtime=%0.4lf\n",
res, res - RESULT, wall1 - wall0);
printf("%u function evaluations.\n", count);

/* Tell the slaves to quit */
for (i = 0; i < nprocs; i++) cnts[i] = -1;
MPI_Scatter(cnts, 1, MPI_INT, &i, 1, MPI_INT, 0, MPI_COMM_WORLD);
}
else { /* Slave */
while (1) {
static double *xsave = NULL;
static int size = 0;
MPI_Scatter(NULL, 0, MPI_INT, &n, 1, MPI_INT, 0, MPI_COMM_WORLD);
if (n < 0) break;
if (n > size) {
if (xsave != NULL) free(xsave);
xsave = malloc((size = n) * sizeof(*xsave));
}
MPI_Scatterv(NULL, 0, 0, MPI_DOUBLE, xsave, n, MPI_DOUBLE, 0,
MPI_COMM_WORLD);
for (i = 0; i < n; i++) xsave[i] = F(xsave[i]);
MPI_Gatherv(xsave, n, MPI_DOUBLE, NULL, 0, 0, MPI_DOUBLE, 0,
MPI_COMM_WORLD);
}
}
MPI_Finalize();
return 0;
}
