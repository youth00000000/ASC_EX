#include <iostream>

template <typename T>
int compare(T x1 , int x2)
{
    T b = x1;
    return (b == x2);
}

int main()
{
    int num1 = 5, num2 = 5;
    std::cout << compare(num1, num2);
}