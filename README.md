# Parallel-Implementation-Reverse-Cuthill-Mckee-Algorithm

## Description
This GitHub project focuses on parallelizing the Reverse Cuthill-McKee algorithm for efficient graph reordering.

<br>
***Version 0 (Sequential):*** This initial version provides a sequential implementation of the Reverse Cuthill-McKee algorithm.

***Version 1 (Parallel Node Neighbors):*** Version 1 introduces parallelism by concurrently finding the neighbor nodes of each vertex in the graph.

***Version 2 (Multi-threaded Processing):*** In Version 2, we take advantage of multi-threading to process multiple nodes simultaneously, further optimizing the algorithm's execution speed.

***Version 3 (Hybrid Parallelization):*** Version 3 combines the strengths of Versions 1 and 2. It aims to reduce the number of idle threads by efficiently managing the queue of nodes to be processed, making the algorithm highly parallelized and responsive to varying workloads.

## Running
Parallel-Implementation-Reverse-Cuthill-McKee-Algorithm/<br>
------- inc/<br>
------- lib/<br>
------- src/<br>
------- input/<br>
------- output/<br>
------- tester/<br>
------- Makefile<br>
<br>

***Build the code:***
```shell
$ make all

```
<br>

***Run the code:***
```shell
$ ./src/v0
$ ./src/v1
$ ./src/v2
$ ./src/v3

```
***or***

```shell
$ make run

```
<br>

***Clean executables and libs***
```shell
$ make clean

```
<br>

***Visualization*** <br>
Run the tester/visualization.py file. 
```shell
$ python tester/visualization.py

```
Need to define the suitable size of the array in the file. 


<br><br>
---

## Results 

### Size = 5000, sparsity = 0.6

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    0.1  |    -    |    -    |
|**verison 1**|    -    |   0.080 |   0.070 |
|**verison 2**|    -    |   0.074 |   0.065 |
|**verison 3**|    -    |   0.070 |   0.061 |


### Size = 5000, sparsity = 0.8

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    0.07 |    -    |    -    |
|**verison 1**|    -    |   0.060 |   0.045 |
|**verison 2**|    -    |   0.057 |   0.043 |
|**verison 3**|    -    |   0.052 |   0.041 |



### Size = 5000, sparsity = 0.9

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    0.05 |    -    |    -    |
|**verison 1**|    -    |   0.046 |   0.040 |
|**verison 2**|    -    |   0.044 |   0.038 |
|**verison 3**|    -    |   0.041 |   0.036 |


<img src=img/part1/5000-0.6.png width=300 height=300/><img src=img/part1/5000-0.8.png width=300 height=300/><img src=img/part1/5000-0.9.png width=300 height=300/>

<br><br>


### Size = 10000, sparsity = 0.6

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    0.4  |    -    |    -    |
|**verison 1**|    -    |   0.31  |   0.22  |
|**verison 2**|    -    |   0.28  |   0.21  |
|**verison 3**|    -    |   0.26  |   0.20  |


### Size = 10000, sparsity = 0.8

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    0.28 |    -    |    -    |
|**verison 1**|    -    |   0.22  |   0.16  |
|**verison 2**|    -    |   0.20  |   0.15  |
|**verison 3**|    -    |   0.19  |   0.145 |


### Size = 10000, sparsity = 0.9

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    0.2  |    -    |    -    |
|**verison 1**|    -    |   0.16  |   0.13  |
|**verison 2**|    -    |   0.145 |   0.12  |
|**verison 3**|    -    |   0.14  |   0.135 |


<img src=img/part2/10000-0.6.png width=310 height=300/><img src=img/part2/10000-0.8.png width=310 height=300/><img src=img/part2/10000-0.9.png width=310 height=300/>
<br><br>



### Size = 20000, sparsity = 0.6

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    1.61 |    -    |    -    |
|**verison 1**|    -    |   1.23  |   0.85  |
|**verison 2**|    -    |   1.12  |   0.83  |
|**verison 3**|    -    |   1.00  |   0.81  |


### Size = 20000, sparsity = 0.8

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    1.09 |    -    |    -    |
|**verison 1**|    -    |   0.83  |   0.62  |
|**verison 2**|    -    |   0.75  |   0.58  |
|**verison 3**|    -    |   0.7   |   0.59  |



### Size = 20000, sparsity = 0.9

|             |  N = 1  |  N = 2  |  N = 4  |
|:-----------:|:-------:|:-------:|:-------:|
|**verison 0**|    0.76 |    -    |    -    |
|**verison 1**|    -    |   0.61  |   0.48  |
|**verison 2**|    -    |   0.57  |   0.47  |
|**verison 3**|    -    |   0.54  |   0.45  |

<img src=img/part3/20000-0.6.png width=310 height=300/><img src=img/part3/20000-0.8.png width=310 height=300/><img src=img/part3/20000-0.9.png width=310 height=300/>

<br><br>

---
## Visualization

<img src=img/vis/vis1.png width=310 height=300/><img src=img/vis/vis2.png width=310 height=300/> 
<br>
<img src=img/vis/vis3.png width=310 height=300/><img src=img/vis/vis4.png width=310 height=300/> 
<br>
<img src=img/vis/vis5.png width=310 height=300/><img src=img/vis/vis6.png width=310 height=300/> 
<br>
<img src=img/vis/vis7.png width=310 height=300/><img src=img/vis/vis8.png width=310 height=300/> 
