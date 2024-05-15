/*
    Task3 <调ucontext库实现版>
    1. 通过ucontext实现一个简单的协程调度器，实现4个协程的切换
    2. 4个协程的切换顺序为A->B->C->D->A->B->C->D->...
    3. 每个协程执行的函数为func，函数内部打印协程名称
*/

#include <stdio.h>
#include <ucontext.h>

#define CO_COUNT 4  // 定义协程数量
#define STACK_SIZE 1024 * 128  // 定义每个协程的栈大小
#define MAX_COUNT 100  // 定义最大的执行次数

ucontext_t uctx_main, uctx_func[CO_COUNT];  // 定义主协程和函数协程的上下文
char func_stack[CO_COUNT][STACK_SIZE];  // 定义每个协程的栈
int current = 0;  // 当前正在运行的协程的索引
char* func_name[CO_COUNT] = {"A", "B", "C", "D"};  // 协程名称
int count = 0;  // 定义全局计数器
int mode = 0;

// 协程函数
void func(void)
{
    while (count < MAX_COUNT)
    {
        printf("this is function %s\n", func_name[current]);
        count++;
        // 切换到下一个协程
        if (mode == 0) 
        {
            if (current == 0) 
            {
                mode = 1;
            }
            current = (current + 1) % CO_COUNT;
        } 
        else 
        {
            current = 2 + (current + 1) % 2;
        }
        /*
            int swapcontext(ucontext_t *oucp, const ucontext_t *ucp)
            oucp：一个指向 ucontext_t 结构体的指针，该结构体将被用来保存当前的上下文
            ucp：一个指向 ucontext_t 结构体的指针，该结构体中的上下文将被恢复并执行
        */
        if (swapcontext(&uctx_func[(current + CO_COUNT - 1) % CO_COUNT], &uctx_func[current]) == -1)
        {
            perror("swapcontext");
            return;
        }
    }
}

int main()
{
    for (int i = 0; i < CO_COUNT; i++)
    {
        // 初始化协程上下文
        /*
            int getcontext(ucontext_t *ucp)
            ucp:一个指向 ucontext_t 结构体的指针，该结构体将被用来保存当前的上下文
        */
        if (getcontext(&uctx_func[i]) == -1)
        {
            perror("getcontext");
            return 1;
        }

        // 设置协程的栈和栈大小
        uctx_func[i].uc_stack.ss_sp = func_stack[i];
        uctx_func[i].uc_stack.ss_size = STACK_SIZE;

        // 修改协程上下文，使得它在运行时会调用 func 函数
        /*
            void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)
            ucp：一个指向 ucontext_t 结构体的指针，该结构体中的上下文将被修改
            func：一个函数指针，指向的函数将被设置为上下文的新的指令指针
            argc：func 的参数数量。
            ...：func 的参数
        */
        makecontext(&uctx_func[i], func, 0);
    }

    // 切换到第一个协程
    if (swapcontext(&uctx_main, &uctx_func[0]) == -1)
    {
        perror("swapcontext");
        return 1;
    }

    return 0;
}