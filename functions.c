#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"
#include <omp.h>


void init_matrix(int* matrix, int size, int mode, double sparsity_limit) {
  if(mode == 1) {
    FILE* file = fopen("input/test_input.txt", "r");
    if(file == NULL)
      exit(0);

    int i=0, j=0;
    int value;
    char ch;


    while(1) {
      ch = fgetc(file);
      value = atoi(&ch);
      if(ch == '0' || ch == '1') {
        *(matrix+i*size+j) = value;
        j++;
        if(j >= size){
          i++;
          j=0;
        }
      }

      if(ch == EOF) {
        break;
      }

    }
    fclose(file);
  }

  if(mode == 2) {
    srand(time(NULL));
    int randNum=0;

    while(1){
      double sum=0;
      for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j <= i; j++) {
          randNum = rand() % 100;
          randNum = randNum<=70 ? 0 : 1; // if randNum >= 70: randNum = 0    else: randNum = 1
          randNum = i==j ? 0 : randNum;
          sum += randNum;
          *(matrix+i*size+j) = randNum;
          *(matrix+j*size+i) = randNum;
        }
      }
      double sparsity = (((double)(size*size)) - sum) / (double)(size*size);
      printf(" >> sparsity: %lf\n", sparsity);
      if(sparsity > sparsity_limit){
        break;
      }
    }
    // write the input array in test_input file for testing
    FILE* file = fopen("input/test_input.txt", "w");
    if(file == NULL)
      exit(0);

    for (size_t i = 0; i < size; i++) {
      for (size_t j = 0; j < size; j++) {
        fprintf(file, "%d, ", *(matrix+i*size+j));
      }
      fprintf(file, "\n");
    }
    fclose(file);
  }
}

int* degreesCalculation(int* matrix, int size) {
  int* degree_array = (int*) malloc(size * sizeof(int));
  int sum=0;

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      sum += *(matrix+ i*size+j);
    }
    degree_array[i] = sum;
    sum = 0;
  }

  return degree_array;
}

int* degreesCalculationParallel(int* matrix, int size, int threads_num) {
  int* degree_array = (int*) malloc(size * sizeof(int));
  int i;

  #pragma omp parallel num_threads(threads_num) private(i)
  {
    int sum=0;
    #pragma omp for schedule(dynamic)
      for ( i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
          sum += *(matrix+ i*size+j);
        }
        degree_array[i] = sum;
        sum = 0;
      }
  }
  return degree_array;
}


int findNotVisited(int* notVisited, int size) {
  for (size_t i = 0; i < size; i++) {
    if(*(notVisited+i) == 1)
      return 1;
  }

  return 0;
}



void swap(int* addr1, int* addr2) {
  int temp = *addr1;
  *addr1 = *addr2;
  *addr2 = temp;
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
      else if(array[j] == array[minIndex] && neighbors[minIndex] > neighbors[j]) {
        minIndex = j;
      }
    }
    swap(&array[minIndex], &array[i]);
    swap(&neighbors[i],&neighbors[minIndex]);
  }

}


void output_write(int* matrix, int rows, int col, char* file_path) {
  FILE* file = fopen(file_path, "w");
  if(file == NULL)
    exit(0);

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < col; j++) {
      fprintf(file, "%d, ", *(matrix+i*col+j));
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

void write_vector(int* vector, int size, char* file_path) {
  FILE* file = fopen(file_path, "w");
  if(file == NULL)
    exit(0);

  for (size_t i = 0; i < size; i++) {
    fprintf(file, "%d, ", *(vector+i));
  }

  fclose(file);
}



void reordering(int** neighbors, queue* Q, int* neighborsCounter, int* R, int* Rsize ,int* currentIndex, int* notVisited, int size, int* queue_size) {

  // printf("Add to Q: ");
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      if(R[*Rsize-size+i] == currentIndex[j] ) {
        for (size_t z = 0; z < neighborsCounter[j]; z++) {
          // printf(" notVisited: %d, \n", notVisited[neighbors[j][z]]);
          if(notVisited[neighbors[j][z]] == 1) {
            // printf(" -> notVisited: %d, \n", notVisited[neighbors[j][z]]);
            queueAdd(Q, neighbors[j][z]);
            notVisited[neighbors[j][z]] = 0;
            *queue_size = *queue_size + 1;
            // printf(" %d ", neighbors[j][z]);
          }
        }
      }
    }
  }


  // printf("  R: ");
  // for (size_t i = 0; i < *Rsize; i++) {
  //   printf("%d, ", R[i]);
  // }

}
