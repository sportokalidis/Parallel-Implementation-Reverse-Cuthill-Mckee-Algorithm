#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <time.h>
#include <sys/time.h>
#include "functions.h"
#include <omp.h>
#include "rcm.h"

#define SIZE 500         // number of rows and cols of sparse array
#define MODE 1           // MODE = 1: Read a sparse array from file, MODE = 2: Create a sparse array, with a specific sparsity
#define SPARSITY 0.6     // the percentage of zeros in sparse array
#define THREADS_NUM 4    // Num of threads


int* CuthillMckee(int* matrix) {
  int* degrees = degreesCalculationParallel(matrix, SIZE, THREADS_NUM);  // degrees points to a vector with the degree of each row

  queue* Q = queueInit();                              // Init the queue
  int* R = (int*) malloc(SIZE * sizeof(int));          // Allocate memory for the permutations array
  if(R == NULL) {
    printf("ERROR: malloc fail");
    exit(1);
  }
  int Rsize=0;                                         // The num of nodes in R
  int* notVisited = (int*) malloc(SIZE*sizeof(int));   // Allocate memory and init notVisited array
  if(notVisited == NULL) {
    printf("ERROR: malloc fail");
    exit(1);
  }
  // init an omp lock
  omp_lock_t writelock;
  omp_init_lock(&writelock);

  // init notVisited array
  for (size_t i = 0; i < SIZE; i++) {
    *(notVisited+i) = 1;
  }


  while(Rsize != SIZE) {
    int minDegreeIndex = -1;

    minDegreeIndex = findMinIdxParallel(degrees, notVisited, SIZE, THREADS_NUM); // find the min degree in parallel
    // minDegreeIndex = findMinIdx(degrees, notVisited, SIZE);

    queueAdd(Q, minDegreeIndex);     // add in Q
    notVisited[minDegreeIndex] = 0;  // This node become visited

    while(!(Q->empty)) {
      int* currentIndex = (int*) malloc(sizeof(int));
      queueDel(Q, currentIndex);
      int* neighbors = (int*) malloc(degrees[*currentIndex] * sizeof(int));
      int neighborsCounter=0;  // the num of neighbors

      int i;
      int CHUNKSIZE = SIZE / THREADS_NUM;

      // find all not visited neighbors in parallel
      #pragma omp parallel num_threads(THREADS_NUM) private(i) shared(notVisited, neighbors)
      {
        #pragma omp for schedule(dynamic, CHUNKSIZE)
          for ( i = 0; i < SIZE; i++) {
            if(i != *currentIndex && *(matrix+(*currentIndex)*SIZE+i)==1 && notVisited[i]==1) {
              omp_set_lock(&writelock);
                neighbors[neighborsCounter++] = i;
                notVisited[i] = 0;
              omp_unset_lock(&writelock);
            }
          }
      }

      // sort the neighbors by degree
      sortByDegree(neighbors, degrees, neighborsCounter);

      // add the sorted neighbor in Q
      for (size_t i = 0; i < neighborsCounter; i++) {
        queueAdd(Q, neighbors[i]);
      }

      // add the current node in permutation array R
      R[Rsize++] = *currentIndex;
      free(currentIndex);
      free(neighbors);
    }

  }

  queueDelete(Q);
  free(notVisited);
  free(degrees);


  return R;
}

//  Reverse  CuthillMckee Result
int* ReverseCuthillMckee(int* matrix) {
  int* rcm = CuthillMckee(matrix);

  int n = SIZE;

  if(SIZE % 2 == 0)
    n -= 1;

  n = n / 2;

  int i;
  int CHUNKSIZE = n / THREADS_NUM;
  #pragma omp parallel num_threads(THREADS_NUM) private(i)
  {
    #pragma omp for schedule(dynamic, CHUNKSIZE)
    for (i = 0; i <= n; i++) {
      swap(&rcm[SIZE - 1 - i], &rcm[i]);
    }
  }

  return rcm;
}


int main(int argc, char const *argv[]) {

  int *matrix = (int*) calloc(SIZE * SIZE, sizeof(int));
  if(matrix == NULL) {
    printf("ERROR: malloc fail");
    exit(1);
  }
  init_matrix(matrix, SIZE, MODE, SPARSITY);

  // printf("MATRIX:\n");
  // for (size_t i = 0; i < SIZE; i++) {
  //   for (size_t j = 0; j < SIZE; j++) {
  //     printf("%d ", *(matrix+i*SIZE+j));
  //   }
  //   printf("\n");
  // }
  // printf("\n");
  //
  // int* degrees = degreesCalculation(matrix, SIZE);
  // printf("DEGREES:\n");
  // for (int i = 0; i < SIZE; i++) {
  //   printf("%d: %d, ", i, degrees[i]);
  // }
  // printf("\n\n");
  // free(degrees);


  struct timeval start, end;

  int* R = (int*) malloc(SIZE*sizeof(int));
  if(R == NULL) {
    printf("ERROR: malloc fail");
    exit(1);
  }

  gettimeofday(&start, NULL);
  R = ReverseCuthillMckee(matrix);
  gettimeofday(&end, NULL);



  // for (size_t i = 0; i < SIZE; i++) {
  //   printf("%d, ", R[i]);
  // }


  double time = ((double)((end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec)))*1e-6;
  printf(" >>> ExecutingTime: %lf sec\n", time);


  write_vector(R, SIZE, "output/v1_output.txt");

  free(matrix);
  free(R);

  return 0;
}
