#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <math.h>
#include <windows.h>
#include "opencl.h"
#pragma warning(disable : 4996)

unsigned char RGB[HEIGHT][WIDTH * 3];
unsigned char record[HEIGHT][WIDTH * 3];
unsigned char result[HEIGHT][WIDTH * 3];

void opencl_run()
{
	int height = 512;
	int width = 512 * 3;
	char *RGB = (char *)malloc(sizeof(char) * height * width);
	char *result = (char *)malloc(sizeof(char) * height * width);

	FILE *read = fopen("TEXT.rgb", "rb");
	FILE *write = fopen("TEXT1.rgb", "wb");
	fread(RGB, sizeof(char), height * width, read);
	size_t datasize = sizeof(char) * height * width;

	cl_int status;
	// 查詢可用平台
	cl_uint numPlatforms = 0;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	// 獲取可用平台
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);

	// 查詢可用設備
	cl_uint numDevices = 0;
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);

	// 獲取可用設備
	cl_device_id *devices;
	devices = (cl_device_id *)malloc(numDevices * sizeof(cl_device_id));
	status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

	// 創建上下文
	cl_context context;
	context = clCreateContext(NULL, numDevices, devices, NULL, NULL, &status);

	// 創建命令隊列
	cl_command_queue cmdQueue;
	cmdQueue = clCreateCommandQueueWithProperties(context, devices[0], 0, &status);

	// 創建緩存空間
	cl_mem buf_rgb;
	buf_rgb = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
	cl_mem buf_result;
	buf_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, datasize, NULL, &status);

	// 將輸入對象寫到主機內存

	status = clEnqueueWriteBuffer(cmdQueue, buf_rgb, CL_TRUE, 0, datasize, RGB, 0, NULL, NULL);
	// status = clEnqueueFillBuffer(cmdQueue, buf_result, &zero, sizeof(zero), 0, datasize, 0, nullptr, nullptr);
	FILE *fp = fopen(FileName, "r");
	fseek(fp, 0, SEEK_END);
	size_t program_size = ftell(fp);
	rewind(fp);

	char *source_str = (char *)malloc(sizeof(char) * program_size + 1);
	source_str[program_size] = '\0';
	fread(source_str, sizeof(char), program_size, fp);
	fclose(fp);

	// 創建程序對象
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&program_size, &status);
	// 編譯程序

	status = clBuildProgram(program, numDevices, devices, NULL, NULL, NULL);

	// 創建內核對象
	cl_kernel kernel;
	kernel = clCreateKernel(program, "MatMul_2D", &status);

	int hc = 512;
	int wc = 512 * 3;
	// 設置內核參數
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf_rgb);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf_result);
	status = clSetKernelArg(kernel, 2, sizeof(cl_int), &hc);
	status = clSetKernelArg(kernel, 3, sizeof(cl_int), &wc);
	// 運行內核

	// size_t globalWorkSize[1] = {(size_t)height * width};
	size_t globalWorkSize[2] = {(size_t)hc, (size_t)wc};

	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	status = clEnqueueNDRangeKernel(cmdQueue, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
	QueryPerformanceCounter(&t2); // 獲取系統計數器
	printf("%-20s:%10.2f ms\n",
		   "opencl Time", (t2.QuadPart - t1.QuadPart) * 1000.0 / tc.QuadPart);
	printf("==================================\n\n");
	// 將輸出讀取到主機內存
	clEnqueueReadBuffer(cmdQueue, buf_result, CL_TRUE, 0, datasize, result, 0, NULL, NULL);

	// 釋放資源
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseContext(context);
	clReleaseMemObject(buf_rgb);
	clReleaseMemObject(buf_result);

	fwrite(result, sizeof(char), height * width, write);
	free(devices);
	free(RGB);
	free(result);
	fclose(read);
	fclose(write);
}

void cpu_run()
{
	FILE *read = fopen("TEXT.rgb", "rb");
	FILE *write = fopen("TEXT1.rgb", "wb");
	fread(RGB, sizeof(unsigned char), HEIGHT * WIDTH * 3, read);
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	for (int c = 0; c < HEIGHT; c++)
	{
		for (int d = 0; d < WIDTH * 3; d++)
		{
			char temp = (-1) * RGB[(c - 1)][d - 3] +
						(-2) * RGB[c][d - 3] +
						(-1) * RGB[c + 1][d - 3] +
						RGB[(c - 1)][d + 3] +
						2 * RGB[(c)][d + 3] +
						RGB[(c + 1)][d + 3];

			char temp1 = (-1) * RGB[(c - 1)][d - 3] +
						 (-2) * RGB[(c - 1)][d] +
						 (-1) * RGB[(c - 1)][d + 3] +
						 RGB[(c + 1)][d - 3] +
						 2 * RGB[(c + 1)][d] +
						 RGB[(c + 1)][d + 3];
			record[c][d] =
				abs(temp) + abs(temp1) > 255 ? (char)255 : (char)abs(temp) + abs(temp1);
		}
	}

	for (int c = 0; c < HEIGHT; c++)
	{
		for (int d = 0; d < WIDTH * 3; d++)
		{
			result[c][d] = record[c][d];
		}
	}
	QueryPerformanceCounter(&t2); // 獲取系統計數器
	printf("%-20s:%10.2f ms\n",
		   "cpu Time", (t2.QuadPart - t1.QuadPart) * 1000.0 / tc.QuadPart);
	printf("==================================\n\n");
	fwrite(result, sizeof(unsigned char), HEIGHT * WIDTH * 3, write);
	fclose(read);
	fclose(write);
}
