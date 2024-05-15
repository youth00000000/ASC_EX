TARGET = matmul

CFLAGS = -std=c++17

# -I 指定头文件路径
CFLAGS += -I/opt/intel/oneapi/mkl/2024.1/include
# -L 指定运行库路径和编译参数
LDFLAGS = -L/opt/intel/oneapi/mkl/2024.1/lib -lmkl_rt 

#LDFLAGS += -lm -lpthread

matmul: matmul.o
	g++ -o $(TARGET) matmul.o  $(LDFLAGS)  

matmul.o: matmul.cpp
	g++ -c matmul.cpp  $(CFLAGS)

clean:
	rm -f *.o $(TARGET)