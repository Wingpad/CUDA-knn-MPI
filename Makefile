# Complier
CC = mpicc
CXX = mpic++
CCC = mpic++
F77 = mpif77
FC = mpif90

# Compiler flags
CFLAGS = -g -lcudart -L$(CUDATOOLKIT_HOME)/lib64 -lrt -lstdc++
CXXFLAGS = -g -std=c++11
CCFLAGS = -g
F77FLAGS = -g
FCFLAGS = -g

NVCC_FLAGS := $(NVCC_FLAGS) -DGPU_MEMPOOL

CHARMC=$(CHARM_HOME)/bin/charmc $(CXXFLAGS) $(OPTS)
OBJS=knn.o kernel.o

# Please fill the execution path of your program here:
EXEC_PATH = 

# Please put hostnames in a file and set the file name in HOST_FILE:
HOST_FILE =            

# Default target.  Always build the C example.  Only build the others
# if Open MPI was build with the relevant language bindings.
MAIN = main
all: $(MAIN)

$(MAIN): $(MAIN).c kernel.o

knn.decl.h knn.def.h: knn.ci
	$(CHARMC) knn.ci

knn.o: knn.cc knn.decl.h knn.def.h
	$(CHARMC) -c knn.cc

knn: $(OBJS)
	$(CHARMC) -language charm++ -o knn $(OBJS)

kernel.o: kernel.cu
	nvcc -o kernel.o -c kernel.cu -lpthread $(NVCC_FLAGS)

run: $(MAIN)
	/usr/local/bin/mpirun -np 2 -hostfile $(HOST_FILE) --mca btl_tcp_if_include eth0 --mca orte_default_hostname $(HOST_FILE) $(EXEC_PATH)/main $(IN) > $(OUT)

clean:
	rm -f $(MAIN) *~ *.o *.decl.h *.def.h charmrun

