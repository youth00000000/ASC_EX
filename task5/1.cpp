#include<iostream>
#include<vector>
#include<mpi.h>

using namespace std;

// 输入：两个一维vector，表示矩阵，输出矩阵的row，middleColumn，column
// 输出：一维vector，表示两个输入vector的乘积
vector<int> matrixMultiplication(vector<int> A, vector<int> B, int row, int middleColumn, int column) {
	vector<int> res(row * column);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			int tmp = 0;
			for (int k = 0; k < middleColumn; k++) {
				// A[i][k] * B[k][j]
				tmp += A[i * middleColumn + k] * B[k * column + j];
			}
			// res[i][j] = tmp
			res[i * column + j] = tmp;
		}
	}
	return res;
}


int main(int argc, char* argv[])

{

	int myrank, processNum;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &processNum);
	MPI_Get_processor_name(processor_name, &namelen);

	// 如果是 0 号进程，随机初始化矩阵数据
	int row = 3000, middleColumn = 2000, column = 3000;
	int farmerProcessNum = processNum - 1; // farmer 进程的数量
	int chunkSize = row / farmerProcessNum; // 每个 farmer 进程处理的行数
	int chunkSizeRemainder = row % farmerProcessNum; // 均分后的余数
	double startTime, endTime; // 记录开始和结束时间
	if (myrank == 0) { // 地主进程
		vector<int> A(row * middleColumn);
		vector<int> B(middleColumn * column);
		// 随机初始化矩阵 A 和 B
		for (int i = 0; i < row * middleColumn; i++) {
			A[i] = rand() % 10;
		}
		for (int i = 0; i < middleColumn * column; i++) {
			B[i] = rand() % 10;
		}

		// 记录程序开始时间
		startTime = MPI_Wtime();

		int startRow = 0, endRow = 0; // 每个 farmer 进程处理的起始行和结束行

		// 将矩阵 A 和 B 分发给其他进程
		for (int i = 1; i < processNum; i++) {
			// 计算下一个进程处理的结束行
			endRow = startRow + chunkSize - 1;
			if (i <= chunkSizeRemainder) {
				endRow++;
			}

			int tmpRow = endRow - startRow + 1;
			// 将矩阵 A 部分的数据分发给其他进程
			MPI_Send(A.data() + startRow * middleColumn, tmpRow * middleColumn, MPI_INT, i, 0, MPI_COMM_WORLD);
			// 将矩阵 B 所有的数据分发给其他进程
			MPI_Send(B.data(), middleColumn * column, MPI_INT, i, 0, MPI_COMM_WORLD);
			// 更新下一个进程处理的起始行
			startRow = endRow + 1;
		}
	}
	else { // 农民进程 (进程号 >= 1)
		// 计算行数
		int tmpRow = chunkSize;

		if (myrank <= chunkSizeRemainder) {
			tmpRow++;
		}

		// 计算列数
		int tmpColumn = column;
		// 计算中间列数
		int tmpMiddleColumn = middleColumn;

		// 接收矩阵 A 的数据
		vector<int> A(tmpRow * tmpMiddleColumn);
		MPI_Recv(A.data(), tmpRow * tmpMiddleColumn, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// 接收矩阵 B 的数据
		vector<int> B(tmpMiddleColumn * tmpColumn);
		MPI_Recv(B.data(), tmpMiddleColumn * tmpColumn, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		// 计算矩阵乘法
		vector<int> res = matrixMultiplication(A, B, tmpRow, tmpMiddleColumn, tmpColumn);

		// 将计算结果返回给 0 号进程
		MPI_Send(res.data(), tmpRow * tmpColumn, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	// 0 号进程接收其他进程的计算结果
	if (myrank == 0) {
		vector<int> res(row * column);
		int startRow = 0, endRow = 0; // 每个 farmer 进程处理的起始行和结束行
		for (int i = 1; i < processNum; i++) {
			// 计算下一个进程处理的结束行
			endRow = startRow + chunkSize - 1;
			if (i <= chunkSizeRemainder) {
				endRow++;
			}
			int tmpRow = endRow - startRow + 1;
			// 将矩阵 A 部分的数据分发给其他进程
			MPI_Recv(res.data() + startRow * column, tmpRow * column, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// 更新下一个进程处理的起始行
			startRow = endRow + 1;
		}

		// 记录程序结束时间
		endTime = MPI_Wtime();
		
		// 打印计算时间
		cout << "Time: " << endTime - startTime << "s" << endl;
	}

	MPI_Finalize();

	return 0;
}