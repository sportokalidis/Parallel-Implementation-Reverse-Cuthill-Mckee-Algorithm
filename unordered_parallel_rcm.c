#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <time.h>
#include <sys/time.h>
#include <omp.h>

#define SIZE 20
#define MODE 1
#define SPARSITY 0.6
#define MAX_THREADS 4

void init_matrix(int* matrix);
int* degreesCalculation(int* matrix);
int findNotVisited(int* notVisited);
void sortByDegree(int* neighbors, int* degrees, int size);
void swap(int* addr1, int* addr2);
void output_write(int* matrix, char* file_path);
void reordering(int** neighbors, queue* Q, int* neighborsCounter, int* R, int* Rsize, int* currentIndex, int* degrees, int size);
void swap_ptr(int* ptr1, int* ptr2);

int start_pos=0;

int* CuthillMckee(int* matrix) {
  int* degrees = degreesCalculation(matrix);

  queue* Q = queueInit();
  int* R = (int*) malloc(SIZE * sizeof(int));
  int Rsize=0;
  int* notVisited = (int*) malloc(SIZE*sizeof(int));
  omp_lock_t writelock;
  omp_init_lock(&writelock);

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

  while(Rsize < SIZE) {
    int minDegreeIndex = 0;

    for (size_t i = 0; i < SIZE; i++) {
      if(degrees[i] < degrees[minDegreeIndex] && notVisited[i] == 1) {
        minDegreeIndex = i;
      }
    }

    queueAdd(Q, minDegreeIndex);
    notVisited[minDegreeIndex] = 0;

    int queue_size = 1;
    int threads_using=queue_size;
    while(!(Q->empty)) {
      int *neighbors[threads_using];
      int neighborsCounter[threads_using];
      int currentIndex[threads_using];
      int tid;

      // printf(">> threads_using: %d\n", threads_using);
      // printf(">> Q->head: %ld, Q->tail: %ld\n", Q->head, Q->tail);
      #pragma omp parallel num_threads(threads_using) private(tid) shared(neighbors, neighborsCounter, currentIndex, notVisited, degrees, Q, queue_size, R)
      {
        tid = omp_get_thread_num();
        //currentIndex[tid] = (int*) malloc(sizeof(int));
        omp_set_lock(&writelock);
          queueDel(Q, &currentIndex[tid]);
          queue_size--;
          R[Rsize] = currentIndex[tid];
          Rsize = Rsize + 1;
        omp_unset_lock(&writelock);

        neighbors[tid] = (int*) malloc(degrees[currentIndex[tid]] * sizeof(int));
        neighborsCounter[tid]=0;

        for (size_t i = 0; i < SIZE; i++) {
          if(i != currentIndex[tid] && *(matrix+(currentIndex[tid])*SIZE+i)==1 && notVisited[i]==1) {
            neighbors[tid][neighborsCounter[tid]] = i;
            neighborsCounter[tid]++;
            // notVisited[i] = 0;
          }
        }
        // queue_size += neighborsCounter[tid];
        sortByDegree(neighbors[tid], degrees, neighborsCounter[tid]);

        // printf("neighbors(id: %d)[cur: %d]: ",tid,currentIndex[tid]);
        // for (size_t i = 0; i < neighborsCounter[tid]; i++) {
        //   printf("tid:%d, %d    ",tid, neighbors[tid][i]);
        // }
        // printf("\n");


      }
      // printf("queue_size : %d,   Q->empty : %d\n", queue_size, Q->empty);
      // if(threads_using > 1) {
        // printf(">> thread > 1, %d\n", threads_using);

      reordering(neighbors, Q, neighborsCounter, R, &Rsize ,currentIndex, notVisited, threads_using);
      // }
      // else if(threads_using == 1){
      //   for (size_t i = 0; i < neighborsCounter[0]; i++) {
      //     queueAdd(Q, neighbors[0][i]);
      //   }
      //
      //   // R[Rsize++] = currentIndex[0];
      //   //free(currentIndex);
      //   //free(neighbors);
      // }
      queue_size = Q->tail - Q->head;
      threads_using = queue_size >= MAX_THREADS ? MAX_THREADS : queue_size;
      // printf("\n\n");
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
    for (size_t j = 0; j < SIZE; j++) {
      swap(matrix+i*SIZE+j, matrix+(SIZE-1-i)*SIZE+j);
    }
  }

  return rcm;
}


int main(int argc, char const *argv[]) {

  int *matrix = (int*) malloc(sizeof(int) * SIZE * SIZE);

  init_matrix(matrix);

  // printf("MATRIX:\n");
  // for (size_t i = 0; i < SIZE; i++) {
  //   for (size_t j = 0; j < SIZE; j++) {
  //     printf("%d ", *(matrix+i*SIZE+j));
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  int* degrees = degreesCalculation(matrix);
  printf("DEGREES:\n");
  for (int i = 0; i < SIZE; i++) {
    printf("%d: %d, ", i, degrees[i]);
  }
  printf("\n\n");
  free(degrees);


  struct timeval start, end;

  int* R = (int*) malloc(SIZE*sizeof(int));


  gettimeofday(&start, NULL);
  R = ReverseCuthillMckee(matrix);
  gettimeofday(&end, NULL);



  for (size_t i = 0; i < SIZE; i++) {
    printf("%d, ", R[i]);
  }


  double time = ((double)((end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec)))*1e-6;
  printf(" >>> ExecutingTime: %lf sec\n", time);

  // output_write(matrix, "output/output.txt");

  free(matrix);
  free(R);

  return 0;
}






// ### FUNCTIONS ###

void init_matrix(int* matrix) {
  if(MODE == 1) {
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
    fclose(file);
  }

  if(MODE == 2) {
    srand(time(NULL));
    int randNum=0;

    while(1){
      double sum=0;
      for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j <= i; j++) {
          randNum = rand() % 100;
          randNum = randNum<=70 ? 0 : 1; // if randNum >= 70: randNum = 0    else: randNum = 1
          randNum = i==j ? 0 : randNum;
          sum += randNum;
          *(matrix+i*SIZE+j) = randNum;
          *(matrix+j*SIZE+i) = randNum;
        }
      }
      double sparsity = (((double)(SIZE*SIZE)) - sum) / (double)(SIZE*SIZE);
      printf(" >> sparsity: %lf\n", sparsity);
      if(sparsity > SPARSITY){
        break;
      }
    }
    // write the input array in test_input file for testing
    FILE* file = fopen("input/test_input.txt", "w");
    if(file == NULL)
      exit(0);

    for (size_t i = 0; i < SIZE; i++) {
      for (size_t j = 0; j < SIZE; j++) {
        fprintf(file, "%d, ", *(matrix+i*SIZE+j));
      }
      fprintf(file, "\n");
    }
    fclose(file);
  }
}

int* degreesCalculation(int* matrix) {
  int* degree_array = (int*) malloc(SIZE * sizeof(int));
  int i;

  #pragma omp parallel private(i)
  {
    int sum=0;
    #pragma omp for schedule(dynamic)
      for ( i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
          sum += *(matrix+ i*SIZE+j);
        }
        degree_array[i] = sum;
        sum = 0;
      }
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

void swap_ptr(int* ptr1, int* ptr2) {
  int* temp = ptr1;
  ptr1 = ptr2;
  ptr2 = temp;
}

void output_write(int* matrix, char* file_path) {
  FILE* file = fopen(file_path, "w");
  if(file == NULL)
    exit(0);

  for (size_t i = 0; i < SIZE; i++) {
    for (size_t j = 0; j < SIZE; j++) {
      fprintf(file, "%d, ", *(matrix+i*SIZE+j));
    }
    fprintf(file, "\n");
  }
  fclose(file);
}


void reordering(int** neighbors, queue* Q, int* neighborsCounter, int* R, int* Rsize ,int* currentIndex, int* notVisited, int size) {


  // for (size_t i = 0; i < *Rsize; i++) {
  //   for (size_t j = 0; j < size; j++) {
  //     // printf("\nR[i] %d == %d currentIndex[j]\n", R[i], currentIndex[j]);
  //     if(R[i] == currentIndex[j]) {
  //       for (size_t z = 0; z < neighborsCounter[j]; z++) {
  //         queueAdd(Q, neighbors[j][z]);
  //         // printf(" >%d ", neighbors[j][z]);
  //       }
  //     }
  //
  //
  //   }
  // }
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
