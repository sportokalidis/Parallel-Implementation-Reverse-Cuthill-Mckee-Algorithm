#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "queue.h"

/*
 * Create or Read a sparse array
 *
 * params: matrix = the sparse array
 *         size = the num of rows and cols of sparse array
 *         mode = if mode=1: Read the matrix from a file, if mode=2 create a sparce array with specific sparsity
 *         sparsity_limit = the down limit of sparsity
 */
void init_matrix(int* matrix, int size, int mode, double sparsity_limit);


/*
 * Calculate the degrees of each row of sparse array
 *
 * params: matrix = the sparse array
 *         size = the num of rows and cols of sparse array
 *
 * Returns a pointer who points to first pos of degrees array
 */
int* degreesCalculation(int* matrix, int size);


/*
 * Same with degreesCalculation(int* matrix, int size)
 * But the calculation become in parallel
*/
int* degreesCalculationParallel(int* matrix, int size, int threads_num);


int findMinIdx(int* degrees, int* notVisited, int size);


int findMinIdxParallel(int* degrees, int* notVisited, int size, int threads_num);

/*
 * Search for not Visited nodes
 * Return 1: if find a notVisited nodes else return 0
 * Using only in debbuging
*/
int findNotVisited(int* notVisited, int size);


/*
 * Take the an array with the neighbors of a node, and sort them by degree
 *
*/
void sortByDegree(int* neighbors, int* degrees, int size);

/*
 * swap the values of two memory address
*/
void swap(int* addr1, int* addr2);

/*
 * Write an integer array in a file. Using to write the input and output array to files
 *
 * params: matrix = the matrix that I want to write
 *         rows, col = Num of row and cols
 *         file_path = the file_path in which write the array
*/
void output_write(int* matrix, int rows, int col, char* file_path);


/*
 * Write an integer vector in a file. Using to write the permutation array to a file.
*/
void write_vector(int* vector, int size, char* file_path);


/*
 * Add the adjacents nodes in queue (Q) with the right order
 * Using in v2, where the nodes are processed in parallel. Then the master thread add the neighbors in Q with the right order using this function
 *
*/
void AddtoQueue(int** neighbors, queue* Q, int* neighborsCounter, int* R, int* Rsize ,int* currentIndex, int* notVisited, int size, int* queue_size);


/*
 * Take the sparse array and the result of RCM algorithm
 * and calculates the final array with the smaller bandwidth
*/
int* reorder(int* matrix, int* R, int size);

#endif
