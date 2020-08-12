#include <stdio.h>
#include <omp.h>


int main(int argc, char const *argv[]) {
  int tid, tid2;
  int counter=0;
  int i;
  int numofthreads, numofthreads2;

  omp_set_dynamic(0);
  omp_set_nested(1);

  #pragma omp parallel num_threads(2) private(tid, numofthreads) shared(counter)
  {
    tid = omp_get_thread_num();
    numofthreads = omp_get_num_threads();

    printf(">> hello I'm the %d out of %d\n", tid, (numofthreads));

    printf("tid == %d\n", tid);
    #pragma omp parallel num_threads(2) private(tid2, i, numofthreads2) shared(counter)
    {
      tid2 = omp_get_thread_num();
      numofthreads2 = omp_get_num_threads();

      printf(">> Second Paral: hello I'm the %d  out of %d\n", tid2, (numofthreads2) );
      counter++;

      #pragma omp for schedule(static, 5) private(tid2,i)
      for (i = 0; i < 10; i++) {
        tid2 = omp_get_thread_num();
        // printf("1\n");
        printf("tid: %d, i: %d\n", tid2, i);
      }
    }
  }

  printf("counter: %d\n", counter);
  return 0;
}


// #include <omp.h>
//
// #include <stdio.h>
// void report_num_threads(int level)
//
// {
//
//     #pragma omp single
//
//     {
//
//          printf("Level %d: number of threads in the team - %d\n",
//
//                       level, omp_get_num_threads());
//
//     }
//
// }
// int main()
// {
//     omp_set_dynamic(1);
//     omp_set_nested(1);
//     #pragma omp parallel num_threads(2)
//     {
//         report_num_threads(1);
//         #pragma omp parallel num_threads(2)
//         {
//             report_num_threads(2);
//             #pragma omp parallel num_threads(2)
//             {
//                 report_num_threads(3);
//             }
//         }
//     }
//     return(0);
// }
