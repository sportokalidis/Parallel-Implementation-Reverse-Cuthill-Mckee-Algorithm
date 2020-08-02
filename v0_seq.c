#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <time.h>
#include <sys/time.h>
#include "functions.h"

#define SIZE 500
#define MODE 2
#define SPARSITY 0.99

int* CuthillMckee(int* matrix) {
  int* degrees = degreesCalculation(matrix, SIZE);

  queue* Q = queueInit();
  int* R = (int*) malloc(SIZE * sizeof(int));
  int Rsize=0;
  int* notVisited = (int*) malloc(SIZE*sizeof(int));

  for (size_t i = 0; i < SIZE; i++) {
    *(notVisited+i) = 1;
  }

  while(Rsize != SIZE) {
    int minDegreeIndex = 0;
    int minDegree = SIZE+10;

    for (size_t i = 0; i < SIZE; i++) {
      if(degrees[i] < minDegree && notVisited[i] == 1) {
        minDegreeIndex = i;
        minDegree = degrees[i];
      }
    }

    queueAdd(Q, minDegreeIndex);
    notVisited[minDegreeIndex] = 0;
    // printf("minDegIndex: %d\n", minDegreeIndex);

    while(!(Q->empty)) {
      int* currentIndex = (int*) malloc(sizeof(int));
      queueDel(Q, currentIndex);
      int* neighbors = (int*) malloc(degrees[*currentIndex] * sizeof(int));
      int neighborsCounter=0;

      for (size_t i = 0; i < SIZE; i++) {
        if(i != *currentIndex && *(matrix+(*currentIndex)*SIZE+i)==1 && notVisited[i]==1) {
          neighbors[neighborsCounter++] = i;
          notVisited[i] = 0;
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


int* reorder(int* matrix, int* R, int size) {
  int* new_matrix =(int*) calloc(size*size, sizeof(int));

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if(*(matrix+size*R[i]+j) == 1) {
        for (int k = 0; k < size; k++) {
          if(R[k] == j) {
            *(new_matrix+size*i+k) = 1;
          }
        }
      }
    }
  }

  return new_matrix;
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


  int* new_matrix = reorder(matrix, R, SIZE);
  output_write(new_matrix, SIZE, SIZE, "output/output_matrix.txt");
  write_vector(R, SIZE, "output/v0_output.txt");

  free(matrix);
  free(R);

  return 0;
}
