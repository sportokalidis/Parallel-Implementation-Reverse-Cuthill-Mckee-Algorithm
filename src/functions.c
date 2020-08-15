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
    char* ch = (char*) malloc(sizeof(char));


    while(1) {
      *ch = fgetc(file);
      if(*ch == '0' || *ch == '1') {
        value = atoi(ch);
        *(matrix+i*size+j) = value;
        j++;
        if(j >= size){
          i++;
          j=0;
        }
      }

      if(*ch == EOF) {
        break;
      }

    }
    fclose(file);
  }

  if(mode == 2) {
    srand(time(NULL));
    // int randNum=0;
    //
    // while(1){
    //   double sum=0;
    //   for (size_t i = 0; i < size; i++) {
    //     for (size_t j = 0; j <= i; j++) {
    //       randNum = rand() % 100;
    //       randNum = randNum<= sparsity_limit*100 ? 0 : 1; // if randNum <= 70: randNum = 0    else: randNum = 1
    //       randNum = i==j ? 0 : randNum;
    //       sum = randNum==1 ? sum+2 : sum;
    //       *(matrix+i*size+j) = randNum;
    //       *(matrix+j*size+i) = randNum;
    //     }
    //   }
    //   double sparsity = (((double)(size*size)) - sum) / (double)(size*size);
    //   printf(" >> sparsity: %lf\n", sparsity);
    //   if(sparsity > sparsity_limit){
    //     break;
    //   }
    // }

    int non_zeros = (size*size) - (size*size*sparsity_limit);
    int sum = 0, randX=0, randY=0;

    for (size_t i = 0; i < non_zeros; i+=2) {
      do {
        randX = rand() % size;
        randY = rand() % size;
      } while(randX == randY);
      *(matrix+randX*size+randY) = 1;
      *(matrix+randY*size+randX) = 1;
      sum += 2;
    }
    double sparsity = 1.0 - ((double)sum)/((double)(size*size));
    printf(" >> Sparsity: %lf\n", sparsity);
    printf(" >> Nonzeros: %d\n", non_zeros);

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
  if(degree_array == NULL) {
    printf("ERROR: malloc fail");
    exit(1);
  }
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


int findMinIdx(int* degrees, int* notVisited, int size) {
  int minDegreeIndex = 0;  // The pos of min degree node in matrix
  int minDegree = size+10; // A node can not have degree > SIZE

  // find the min degree
  for (size_t i = 0; i < size; i++) {
    if(degrees[i] < minDegree && notVisited[i] == 1) {
      minDegreeIndex = i;
      minDegree = degrees[i];
    }
  }

  return minDegreeIndex;
}


int findMinIdxParallel(int* degrees, int* notVisited, int size, int threads_num) {
  int degArray[threads_num];
  int idxArray[threads_num];
  int tid;

  omp_lock_t writelock;
  omp_init_lock(&writelock);

  // find the min degree
  int i;
  int CHUNKSIZE = size / threads_num;
  #pragma omp parallel num_threads(threads_num) private(i, tid)
  {
    tid = omp_get_thread_num();
    int minDegreeIndex = -1;  // The pos of min degree node in matrix
    int minDegree = size+10; // A node can not have degree > SIZE
    #pragma omp for schedule(static, CHUNKSIZE)
      for (i = 0; i < size; i++) {
        if(degrees[i] < minDegree && notVisited[i] == 1) {
          minDegreeIndex = i;
          minDegree = degrees[i];
        }
      }
      degArray[tid] = minDegree;
      idxArray[tid] = minDegreeIndex;
  }

  int globalMinIdx = idxArray[0];
  int globalMinDeg = degArray[0];

  for (size_t i = 1; i < threads_num; i++) {
    if(degArray[i] < globalMinDeg) {
      globalMinDeg = degArray[i];
      globalMinIdx = idxArray[i];
    }
  }

  return globalMinIdx;
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



void AddtoQueue(int** neighbors, queue* Q, int* neighborsCounter, int* R, int* Rsize ,int* currentIndex, int* notVisited, int size, int* queue_size) {

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
