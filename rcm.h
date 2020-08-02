#ifndef RCM_H
#define RCM_H

/*
 * Implementation of Cuthill-McKee algorithm
 *
 * Param: matrix = the sparse array
 *
 * Returns a integer pointer to permutation array R
 *
*/
int* CuthillMckee(int* matrix);

/*
 * Implementation of Reverse-Cuthill-McKee algorithm
 * Use the Cuthill-McKee result and reverse it
 *
 *
 * Param: matrix = the sparse array
 *
 * Returns a integer pointer to permutation array R
 *
*/
int* ReverseCuthillMckee(int* matrix);


/*
 * Same with the ReverseCuthillMckee(int* matrix)
 * But reverse the result in parallel
*/
int* ReverseCuthillMckeeParallel(int* matrix);


#endif
