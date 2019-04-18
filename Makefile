all: first-kernel.c
	gcc -o first-kernel first-kernel.c -lOpenCL

clean:
	rm first-kernel
