#include <cstdint>
#include <stdint.h> 
#include <stdio.h>


struct thread_t { 
    int64_t rsp; 
    int64_t pc; 
    int64_t stack[1024]; 
};

void function_a(); 
void function_b(); 
void function_c(); 
void function_d(); 

void init_thread(struct thread_t* thread, void (*func)()) {
    // 将函数指针放在栈底，模拟调用时的返回地址
    thread->stack[1023] = (int64_t)func;
    // 将栈指针 rsp 指向栈底的返回地址
    thread->rsp = (int64_t)&thread->stack[1023];
    // 程序计数器 pc 指向函数的第一条指令
    thread->pc = (int64_t)func;
}

void switch_thread(struct thread_t* from, struct thread_t* to) {

    if (from -> pc == (int64_t)function_b && to -> pc == (int64_t)function_a)
    {
        to -> pc = (int64_t)function_c;
    }
    if (from -> pc == (int64_t)function_c && to -> pc == (int64_t)function_b)
    {
        to -> pc = (int64_t)function_d;
    }

    if ((from -> pc == (int64_t)function_c && to -> pc == (int64_t)function_d))
    {
        __asm__ volatile (
                "popq %%rsp" 
                : "=m" (from->rsp)
        );

        __asm__ volatile (
                "pushq %%rsp" 
                : 
                : "m" (to->rsp)
        );

        __asm__ volatile (
                "jmpq *%0"
                :
                : "m" (to->pc) 
        );
    }
    else
    {
        // 保存当前线程的 rsp
        __asm__ volatile (
                "movq %%rsp, %0" 
                : "=m" (from->rsp)
        );

        // 设置新线程的 rsp
        __asm__ volatile (
                "movq %0, %%rsp" 
                : 
                : "m" (to->rsp)
        );

        // 跳转到新线程的 pc
        __asm__ volatile (
                "jmpq *%0" 
                : 
                : "m" (to->pc)
        );
    }
    
}

struct thread_t thread_a; 
struct thread_t thread_b; 

int main() { 
    init_thread(&thread_a, function_a); 
    init_thread(&thread_b, function_b); 
    struct thread_t self; 
    switch_thread(&self, &thread_a); 
    return 0; 
}

void function_a() {
    printf("this is function A\n"); 
    function_c(); 
}

void function_b() { 
    printf("this is function B\n"); 
    function_d(); 
}

void function_c() { 
    printf("this is function C\n"); 
    switch_thread(&thread_a, &thread_b); 
}

void function_d() { 
    printf("this is function D\n"); 
    switch_thread(&thread_b, &thread_a); 
}