#include <CL/opencl.h>
#include <stdio.h>

static const char *readFile(const char *file) {
    FILE *f = fopen(file, "rb");
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = malloc(length + 1);
    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    fclose(f);
    return buffer;
}

int main() {
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);
    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, NULL, NULL);
    const char *source = readFile("kernel.cl");
    cl_program program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "main", NULL);
    cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 4 * sizeof(int), NULL, NULL);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
    const size_t globalWorkSize = 4, localWorkSize = 1;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    clFinish(queue);
    int values[4];
    clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, 4 * sizeof(int), values, 0, NULL, NULL);
    for (int i = 0; i < 4; i++) {
        printf("%d\n", values[i]);
    }
    clReleaseMemObject(buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    return 0;
}
