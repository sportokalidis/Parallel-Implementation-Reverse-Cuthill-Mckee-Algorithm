#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <time.h>
#include <sys/time.h>
#include "functions.h"
#include <omp.h>

#define SIZE 1000
#define MODE 1
#define SPARSITY 0.9
#define THREADS_NUM 4


int* CuthillMckee(int* matrix) {
  int* degrees = degreesCalculationParallel(matrix, SIZE, THREADS_NUM);

  queue* Q = queueInit();
  int* R = (int*) malloc(SIZE * sizeof(int));
  int Rsize=0;
  int* notVisited = (int*) malloc(SIZE*sizeof(int));
  omp_lock_t writelock;
  omp_init_lock(&writelock);

  for (size_t i = 0; i < SIZE; i++) {
    *(notVisited+i) = 1;
  }


  while(Rsize != SIZE) {
    int minDegreeIndex = -1;
    int minDegree = SIZE+10;

    for (size_t i = 0; i < SIZE; i++) {
      if(degrees[i] < minDegree && notVisited[i] == 1) {
        minDegreeIndex = i;
        minDegree = degrees[i];
      }
    }

    queueAdd(Q, minDegreeIndex);
    notVisited[minDegreeIndex] = 0;

    while(!(Q->empty)) {
      int* currentIndex = (int*) malloc(sizeof(int));
      queueDel(Q, currentIndex);
      int* neighbors = (int*) malloc(degrees[*currentIndex] * sizeof(int));
      int neighborsCounter=0;

      int i;
      int CHUNKSIZE = SIZE / THREADS_NUM;
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

      sortByDegree(neighbors, degrees, neighborsCounter);

      // printf("sorted neighbors of %d: ", *currentIndex);
      // for (size_t i = 0; i < neighborsCounter; i++) {
      //   printf("%d, ", neighbors[i]);
      // }
      // printf("\n");


      for (size_t i = 0; i < neighborsCounter; i++) {
        queueAdd(Q, neighbors[i]);
      }

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

int* ReverseCuthillMckee(int* matrix) {
  int* rcm = CuthillMckee(matrix);

  int n = SIZE;

  if(SIZE % 2 == 0)
    n -= 1;

  n = n / 2;

  for (size_t i = 0; i <= n; i++) {
    swap(&rcm[SIZE - 1 - i], &rcm[i]);
  }

  return rcm;
}


int main(int argc, char const *argv[]) {

  int *matrix = (int*) calloc(SIZE * SIZE, sizeof(int));

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


  gettimeofday(&start, NULL);
  R = ReverseCuthillMckee(matrix);
  gettimeofday(&end, NULL);



  // for (size_t i = 0; i < SIZE; i++) {
  //   printf("%d, ", R[i]);
  // }


  double time = ((double)((end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec)))*1e-6;
  printf(" >>> ExecutingTime: %lf sec\n", time);

  // output_write(matrix, SIZE, SIZE, "output/output.txt");
  write_vector(R, SIZE, "output/v1_output.txt");

  free(matrix);
  free(R);

  return 0;
}
