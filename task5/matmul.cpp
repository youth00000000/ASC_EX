#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "mkl.h"

long int Mkltime, Opttime;

class matrix {
public:
    friend bool check(const matrix &a, const matrix &b);
    friend std::ostream& operator<<(std::ostream& out, const matrix &m);

    matrix(int rows, int cols)
    {
        this->rows = rows;
        this->cols = cols;
        this->data = new double[rows * cols]();
        for(int i = 0; i < rows * cols; i++)
            // data[i] = 1;
            data[i] = (double)rand() / RAND_MAX;
    }

    ~matrix()
    {
        delete[] this->data;
    }

    matrix matmul_mkl(const matrix &a) const
    {
        matrix c = matrix(this->rows, a.cols);

        struct timeval start, end;

        gettimeofday(&start, NULL);

        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, this->rows, a.cols, this->cols,
                    1.0, this->data, this->cols, a.data, a.cols, 0.0, c.data, c.cols);


        gettimeofday(&end, NULL);

        Mkltime =  (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

        printf("[MKL Matmul] %ldms\n", Mkltime);

        return c;
    }

    matrix matmul_optim(const matrix &a) const
    {
        matrix c = matrix(this->rows, a.cols);

        struct timeval start, end;

        gettimeofday(&start, NULL);

        // Modify Code below!

        for(int i = 0; i < this->rows; i++) {
            for(int j = 0; j < a.cols; j++) {
                double sum = 0;
                for (int k = 0; k < this->cols; k++) {
                    sum += this->data[i * this->cols + k] * a.data[k * a.cols + j];
                }
                c.data[i * c.cols + j] = sum;
            }
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
