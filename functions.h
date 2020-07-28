#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "queue.h"

void init_matrix(int* matrix, int size, int mode, double sparsity_limit);

int* degreesCalculation(int* matrix, int size);

int* degreesCalculationParallel(int* matrix, int size, int threads_num);

int findNotVisited(int* notVisited, int size);

void sortByDegree(int* neighbors, int* degrees, int size);

void swap(int* addr1, int* addr2);

void output_write(int* matrix, int rows, int col, char* file_path);

void write_vector(int* vector, int size, char* file_path);

void reordering(int** neighbors, queue* Q, int* neighborsCounter, int* R, int* Rsize ,int* currentIndex, int* notVisited, int size, int* queue_size);


#endif
