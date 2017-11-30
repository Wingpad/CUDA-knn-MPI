#ifndef KERNEL_H
#define KERNEL_H

// void launch(int id, int m, int n, int k, int *V, int *out);
void kernelSetup(void *cb, int tid, int m, int n, int k, int* h_A, int* &h_B);

#endif
