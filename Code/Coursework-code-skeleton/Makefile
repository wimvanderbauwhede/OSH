CFLAGS = -std=c99 -Wall -Wno-int-to-pointer-cast -Wno-unused-variable -Wno-deprecated-declarations -DMAX_ITERS=100 -DNTH=8 -DBUF_SZ=64 -DVERBOSE_MT=0 -DW1=6 -DW2=7

CC = gcc
ALL = testbench

all: $(ALL)


testbench: testbench.o driver.o
	$(CC) testbench.o driver.o -lOpenCL -pthread -o testbench


testbench.o: testbench.c
	$(CC) $(CFLAGS) -pthread -c testbench.c 

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

clean:
	rm *.o testbench
