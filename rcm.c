#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <time.h>

#define SIZE 10
#define MODE 2
#define SPARSITY 0.5

void init_matrix(int* matrix);
int* degreesCalculation(int* matrix);
int findNotVisited(int* notVisited);
void sortByDegree(int* neighbors, int* degrees, int size);
void swap(int* addr1, int* addr2);


int* CuthillMckee(int* matrix) {
  int* degrees = degreesCalculation(matrix);

  queue* Q = queueInit();
  int* R = (int*) malloc(SIZE * sizeof(int));
  int Rsize=0;
  int* notVisited = (int*) malloc(SIZE*sizeof(int));

  for (size_t i = 0; i < SIZE; i++) {
    *(notVisited+i) = 1;
  }


  /*
  int* notVisited = (int*) malloc( 2*SIZE * sizeof(int));
  for (size_t i = 0; i < SIZE; i++) {
    for (size_t j = 0; j < 2; j++) {
      *(notVisited+i*2+j) = j==0 ? i
                                 : *(degrees+i);
    }
  }
  */

  while(findNotVisited(notVisited)) {
    int minDegreeIndex = 0;

    for (size_t i = 0; i < SIZE; i++) {
      if(degrees[i] < degrees[minDegreeIndex] && notVisited[i] == 1) {
        minDegreeIndex = i;
      }
    }

    queueAdd(Q, minDegreeIndex);
    notVisited[minDegreeIndex] = 0;

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
      /*
      printf("neighbors: ");
      for (size_t i = 0; i < neighborsCounter; i++) {
        printf("%d, ", neighbors[i]);
      }
      printf("\n");
      */
      for (size_t i = 0; i < neighborsCounter; i++) {
        queueAdd(Q, neighbors[i]);
      }

      R[Rsize++] = *currentIndex;
      free(currentIndex);
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

  int *matrix = (int*) malloc(sizeof(int) * SIZE * SIZE);

  init_matrix(matrix);
  /*
  printf("MATRIX:\n");
  for (size_t i = 0; i < SIZE; i++) {
    for (size_t j = 0; j < SIZE; j++) {
      printf("%d ", *(matrix+i*SIZE+j));
    }
    printf("\n");
  }
  printf("\n");

  int* degrees = degreesCalculation(matrix);
  printf("DEGREES:\n");
  for (size_t i = 0; i < SIZE; i++) {
    printf("%d, ", degrees[i]);
  }
  printf("\n\n");
  free(degrees);
  */

  int* R = (int*) malloc(SIZE*sizeof(int));
  R = ReverseCuthillMckee(matrix);

  for (size_t i = 0; i < SIZE; i++) {
    printf("%d, ", R[i]);
  }

  free(matrix);
  free(R);

  return 0;
}






// ### FUNCTIONS ###

void init_matrix(int* matrix) {
  if(MODE == 1) {
    FILE* file = fopen("input.txt", "r");
    int i=0, j=0;
    int value;
    char ch;


    while(1) {
      ch = fgetc(file);
      value = atoi(&ch);
      if(ch == '0' || ch == '1') {
        *(matrix+i*SIZE+j) = value;
        j++;
        if(j >= SIZE){
          i++;
          j=0;
        }
      }

      if(ch == EOF) {
        break;
      }

    }
  }

  if(MODE == 2) {
    srand(time(NULL));
    int randNum=0;

    while(1){
      double sum=0;
      for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
          randNum = rand() % 100;
          randNum = randNum>=50 ? 0 : 1; // if randNum >= 70: randNum = 0    else: randNum = 1
          sum += randNum;
          *(matrix+i*SIZE+j) = randNum;
        }
      }
      double sparsity = (((double)(SIZE*SIZE)) - sum) / (double)(SIZE*SIZE);
      printf(" >> sparsity: %lf\n", sparsity);
      if(sparsity > SPARSITY){
        break;
      }
    }
  }
}

int* degreesCalculation(int* matrix) {
  int* degree_array = (int*) malloc(SIZE * sizeof(int));
  int sum=0;

  for (size_t i = 0; i < SIZE; i++) {
    for (size_t j = 0; j < SIZE; j++) {
      sum += *(matrix+ i*SIZE+j);
    }
    degree_array[i] = sum;
    sum = 0;
  }

  return degree_array;
}


int findNotVisited(int* notVisited) {
  for (size_t i = 0; i < SIZE; i++) {
    if(*(notVisited+i) == 1)
      return 1;
  }

  return 0;
}


void sortByDegree(int* neighbors, int* degrees, int size) {
  int array[size];

  for (size_t i = 0; i < size; i++) {
    array[i] = degrees[neighbors[i]];
  }

  int minIndex = 0;

  for (size_t i = 0; i < size; i++) {
    minIndex = i;
    for (size_t j = i+1; j < size; j++) {
      if(array[j] < array[minIndex]) {
        minIndex = j;
      }
    }
    swap(&array[minIndex], &array[i]);
    swap(&neighbors[i],&neighbors[minIndex]);
  }

}

void swap(int* addr1, int* addr2) {
  int temp = *addr1;
  *addr1 = *addr2;
  *addr2 = temp;
}
