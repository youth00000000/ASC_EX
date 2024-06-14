#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "mkl.h"
#include "omp.h"
#include <thread>
#include <vector>
#include <pthread.h>
#include "mpi.h"

long int Mkltime, Opttime;

class matrix {
public:
    friend bool check(const matrix &a, const matrix &b);
    friend std::ostream& operator<<(std::ostream& out, const matrix &m);

    //构造函数
    matrix(int rows, int cols)
    {
        this->rows = rows;
        this->cols = cols;
        this->data = new double[rows * cols]();//new数组初始化为0
        for(int i = 0; i < rows * cols; i++)
            // data[i] = 1;
            data[i] = (double)rand() / RAND_MAX; //随机数初始化, 0~1
    }

    ~matrix()
    {
        delete[] this->data;
    }

    //矩阵乘法 mkl优化
    matrix matmul_mkl(const matrix &a) const
    {
        matrix c = matrix(this->rows, a.cols);

        struct timeval start, end; //计时

        gettimeofday(&start, NULL/*获取时区一般不用*/); //开始计时

        //mkl双精度浮点数矩阵乘法库函数
        cblas_dgemm(CblasRowMajor/*行主序*/, CblasNoTrans/*A不转置*/, CblasNoTrans/*B不转置*/, this->rows/*指定列数*/, a.cols, this->cols,
                    1.0/*乘法缩放因子*/, this->data, this->cols, a.data, a.cols, 0.0/*加法缩放因子*/, c.data, c.cols);


        gettimeofday(&end, NULL);//结束计时

        Mkltime =  (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;//转成ms

        printf("[MKL Matmul] %ldms\n", Mkltime);

        return c;
    }


    //矩阵乘法 std::thread优化 4.8913%~4.26752%
    matrix matmul_optim(const matrix &a) const
    {
        matrix c = matrix(this->rows, a.cols);

        struct timeval start, end;

        gettimeofday(&start, NULL);

        // Modify Code below!

        int num_threads = std::thread::hardware_concurrency(); // 获取硬件支持的并发线程数
        std::vector<std::thread> threads(num_threads); // 创建线程数组

        //#pragma omp parallel for //std::thread内部使用openmp 优化线程分配 3.9845%
        for(int t = 0; t < num_threads; t++) 
        {
            // 使用lambda表达式创建线程
            //std::thread内部使用openmp 优化单线程内部分配 2.0384%
            threads[t] = std::thread([&, t]() 
            { 
                //#pragma omp parallel for  //std::thread内部使用openmp 优化单线程内部分配 2.0384%
                for(int i = t; i < this->rows; i += num_threads) 
                {
                    //#pragma omp parallel for //std::thread内部使用openmp 双优化线程运算 0.223934%
                    for(int j = 0; j < a.cols; j++) 
                    {
                        double sum = 0;
                        //#pragma omp parallel for //跑不出来...
                        for (int k = 0; k < this->cols; k++) 
                        {
                            sum += this->data[i * this->cols + k] * a.data[k * a.cols + j];
                        }
                        c.data[i * c.cols + j] = sum;
                    }
                }
            }
            );
        }

        for(auto& thread : threads) 
        {
            thread.join(); // 等待所有线程完成
        }

        // Modify Code above

        gettimeofday(&end, NULL);

        Opttime = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

        printf("[Optim Matmul] %ldms\n", Opttime);

        return c;
    }

    
    
private:
    double *data;
    int rows;
    int cols;
};

//比较两矩阵是否相等
bool check(const matrix &a, const matrix &b)
{
    if(a.rows != b.rows || a.cols != b.cols)
        return false;

    for(int i = 0; i < a.rows * a.cols; i++) {
        if (a.data[i] - b.data[i] > 1e-7)
            return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const matrix &m)
{
    for(int i = 0; i < m.cols * m.rows; i++) {
        if(i % 10 == 0) out << std::endl;
        out << m.data[i] << " ";
    }
    return out;
}


int main()
{
    matrix a = matrix(1000, 2000);
    matrix b = matrix(2000, 3000);
    matrix ans = a.matmul_mkl(b);

    // std::cout << a << std::endl;
    // std::cout << b << std::endl;
    // std::cout << ans << std::endl;

    matrix c = a.matmul_optim(b);
    // std::cout << c;
    if (!check(c, ans)) {
        std::cout << "answer wrong!" << std::endl;
    } else {
        std::cout << "answer correct!" << std::endl;
        std::cout << "Your speedup: " << ((double)Mkltime / Opttime) * 100 << "%" << std::endl;
    }

    return 0;
}
