# Progetto d'Esame di Calcolo Parallelo e Distribuito

## Descrizione

Questo repository contiene il codice per un progetto del corso di Calcolo Parallelo e Distribuito. Il progetto include sia implementazioni sequenziali che parallele di un algoritmo di somma di vettori utilizzando MPI (Message Passing Interface).

## Requisiti

- Implementazione di MPI (ad esempio, MPICH, OpenMPI)
- Compilatore C (ad esempio, GCC)

## Installazione

1. Clonare il repository:
    ```sh
    git clone https://github.com/yourusername/parallel-distributed-computing-exam.git
    cd parallel-distributed-computing-exam
    ```

2. Compilare il codice:
    ```sh
    mpicc -o vector_sum vector_sum.c
    ```

## Uso

Per eseguire il programma, usare il seguente comando:
```sh
mpirun -np <numero_di_processi> ./vector_sum <M>
```

- `<numero_di_processi>`: Il numero di processi da utilizzare nel calcolo parallelo.
- `<M>`: La dimensione di ogni vettore.

Esempio:
```sh
mpirun -np 4 ./vector_sum 1000
```

## Funzioni

### `sequential_algorithm(int P, int M, double *seq_time)`
Questa funzione esegue la versione sequenziale dell'algoritmo di somma dei vettori.

- **Parametri**:
  - `P`: Numero di vettori.
  - `M`: Dimensione di ogni vettore.
  - `seq_time`: Puntatore per memorizzare il tempo di esecuzione.

### `parallel_algorithm(int M, double *par_time)`
Questa funzione esegue la versione parallela dell'algoritmo di somma dei vettori utilizzando MPI.

- **Parametri**:
  - `M`: Dimensione di ogni vettore.
  - `par_time`: Puntatore per memorizzare il tempo di esecuzione.

### `main(int argc, char *argv[])`
La funzione principale inizializza MPI, analizza gli argomenti della riga di comando e chiama le funzioni appropriate per il calcolo sequenziale e parallelo.

## Risultati

Il programma stampa le somme locali calcolate da ciascun processo e i tempi di esecuzione sia per i calcoli sequenziali che paralleli. I risultati vengono inoltre salvati in un file denominato `time.txt` nel seguente formato:

```
<M>,<numero_di_processi>,<tempo_sequenziale>,<tempo_parallelo>
```
---

# Parallel and Distributed Computing Exam Project

## Description

This repository contains the code for a project in the course of Parallel and Distributed Computing. The project includes both sequential and parallel implementations of a vector sum algorithm using MPI (Message Passing Interface).

## Requirements

- MPI implementation (e.g., MPICH, OpenMPI)
- C compiler (e.g., GCC)

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/parallel-distributed-computing-exam.git
    cd parallel-distributed-computing-exam
    ```

2. Compile the code:
    ```sh
    mpicc -o vector_sum vector_sum.c
    ```

## Usage

To run the program, use the following command:
```sh
mpirun -np <number_of_processes> ./vector_sum <M>
```

- `<number_of_processes>`: The number of processes to use in the parallel computation.
- `<M>`: The size of each vector.

Example:
```sh
mpirun -np 4 ./vector_sum 1000
```

## Functions

### `sequential_algorithm(int P, int M, double *seq_time)`
This function executes the sequential version of the vector sum algorithm.

- **Parameters**:
  - `P`: Number of vectors.
  - `M`: Size of each vector.
  - `seq_time`: Pointer to store the execution time.

### `parallel_algorithm(int M, double *par_time)`
This function executes the parallel version of the vector sum algorithm using MPI.

- **Parameters**:
  - `M`: Size of each vector.
  - `par_time`: Pointer to store the execution time.

### `main(int argc, char *argv[])`
The main function initializes MPI, parses command-line arguments, and calls the appropriate functions for sequential and parallel computation.

## Results

The program outputs the local sums computed by each process and the execution times for both sequential and parallel computations. The results are also saved in a file named `time.txt` in the following format:

```
<M>,<number_of_processes>,<sequential_time>,<parallel_time>
```
