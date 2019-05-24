# 并行计算导论第二部分
## 自适应积分
### 递归串行程序
main.c integration.c function.c
> 其中integration.c实现梯形公式递归计算积分值函数
> function.c实现被积函数与返回当前CPU和墙上时间函数
### 递归并行算法（各线程简单均分求解区间）
main-mpi.c integration.c function.c
> 负载均衡效率低
### 基于主从模式的并行算法
main_nr.c main_nr-mpi1.c main_nr-mpi2.c integration_nr.c function.c
> main_nr.c和main_nr-mpi1.c & main_nr-mpi2.c分别是串行和并行主程序实现(使用非阻塞通信&基于散发/收集通信)

# USAGE
参看Makefile
