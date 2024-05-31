#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Funzione per eseguire l'algoritmo sequenziale
void sequential_algorithm(int P, int M, double *seq_time)
{
	int **vectors = malloc(P * sizeof(int *));  // Allocazione memoria per i vettori
	int *local_sums = malloc(P * sizeof(int));  // Allocazione memoria per le somme locali
	clock_t start_time;
	clock_t end_time;
	double elapsed_time;

	// Allocazione memoria per ogni vettore
	for (int i = 0; i < P; i++)
	{
		vectors[i] = malloc(M * sizeof(int));
	}

	// Riempimento dei vettori con numeri casuali
	for (int p = 0; p < P; p++)
	{
		srand(p);  // Inizializzazione del generatore di numeri casuali con il seme p
		for (int i = 0; i < M; i++)
		{
			vectors[p][i] = rand() % 10000;  // Generazione di numeri casuali tra 0 e 9999
		}
	}

	start_time = clock();  // Inizio del cronometro

	// Calcolo della somma locale per ogni vettore
	for (int p = 0; p < P; p++)
	{
		local_sums[p] = 0;  // Inizializzazione della somma locale
		for (int i = 0; i < M; i++)
		{
			local_sums[p] += vectors[p][i];  // Somma degli elementi del vettore
		}
	}

	end_time = clock();  // Fine del cronometro
	elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;  // Calcolo del tempo trascorso

	// Stampa delle somme locali
	for (int p = 0; p < P; p++)
	{
		printf("Numero del vettore %d: Somma Locale: %d\n", p, local_sums[p]);
	}

	// Stampa dell'array generale delle somme
	printf("Array Generale delle somme: [");
	for (int p = 0; p < P; p++)
	{
		printf(" %d ", local_sums[p]);
	}
	printf("]\n");

	// Stampa del tempo di esecuzione sequenziale
	printf("Tempo di esecuzione sequenziale: %f secondi\n\n", elapsed_time);
	*seq_time = elapsed_time;  // Salvataggio del tempo di esecuzione

	// Deallocazione della memoria
	for (int i = 0; i < P; i++)
	{
		free(vectors[i]);
	}
	free(vectors);
	free(local_sums);
}

// Funzione per eseguire l'algoritmo parallelo
void parallel_algorithm(int M, double *par_time)
{
	int *vector;
	int *all_sums;
	int local_sum = 0;
	int size;
	int rank;
	double time_start;
	double time_finish;
	double elapsed_time;
	double max_elapsed_time;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Ottenere il rango del processo
	MPI_Comm_size(MPI_COMM_WORLD, &size);  // Ottenere il numero totale di processi

	vector = malloc(M * sizeof(int));  // Allocazione memoria per il vettore locale
	all_sums = malloc(size * sizeof(int));  // Allocazione memoria per le somme di tutti i processi

	srand(rank);  // Inizializzazione del generatore di numeri casuali con il rango del processo
	for (int i = 0; i < M; i++)
	{
		vector[i] = rand() % M;  // Riempimento del vettore con numeri casuali tra 0 e M-1
	}

	MPI_Barrier(MPI_COMM_WORLD);  // Sincronizzazione dei processi
	time_start = MPI_Wtime();  // Inizio del cronometro

	// Calcolo della somma locale
	for (int i = 0; i < M; i++)
	{
		local_sum += vector[i];
	}

	// Raccolta delle somme locali di tutti i processi
	MPI_Allgather(&local_sum, 1, MPI_INT, all_sums, 1, MPI_INT, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);  // Sincronizzazione dei processi
	time_finish = MPI_Wtime();  // Fine del cronometro
	elapsed_time = time_finish - time_start;  // Calcolo del tempo trascorso

	// Riduzione per ottenere il tempo massimo tra i processi
	MPI_Reduce(&elapsed_time, &max_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	// Stampa della somma locale per ogni processore
	printf("Processore %d: Somma Locale: %d\n", rank, local_sum);

	if (rank == 0)
	{
		// Stampa dell'array generale delle somme
		printf("Array Generale delle somme: [");
		for (int p = 0; p < size; p++)
		{
			printf(" %d ", all_sums[p]);
		}
		printf("]\n");

		// Stampa del tempo di esecuzione parallela
		printf("Tempo di esecuzione parallela (max): %f secondi\n\n", max_elapsed_time);
		*par_time = max_elapsed_time;  // Salvataggio del tempo di esecuzione
	}

	// Deallocazione della memoria
	free(vector);
	free(all_sums);
}

int main(int argc, char *argv[])
{
	int rank, size, M;
	double seq_time = 0.0;
	double par_time = 0.0;

	MPI_Init(&argc, &argv);  // Inizializzazione di MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Ottenere il rango del processo
	MPI_Comm_size(MPI_COMM_WORLD, &size);  // Ottenere il numero totale di processi

	if (argc != 2)
	{
		if (rank == 0)
		{
			printf("Uso: %s <M>\n", argv[0]);
		}
		MPI_Finalize();  // Terminazione di MPI
		return 1;
	}

	M = atoi(argv[1]);  // Conversione dell'argomento in un intero
	if (M <= 0)
	{
		if (rank == 0)
		{
			printf("M deve essere un intero positivo.\n");
		}
		MPI_Finalize();  // Terminazione di MPI
		return 1;
	}

	if (rank == 0)
	{
		sequential_algorithm(size, M, &seq_time);  // Esecuzione dell'algoritmo sequenziale
	}
	parallel_algorithm(M, &par_time);  // Esecuzione dell'algoritmo parallelo

	if (rank == 0)
	{
		FILE *f = fopen("time.txt", "a");  // Apertura del file in modalità append
		if (f == NULL)
		{
			f = fopen("time.txt", "w");  // Creazione del file se non esiste
			if (f == NULL)
			{
				printf("Errore nell'apertura del file!\n");
				exit(1);
			}
		}
		fprintf(f, "%d,%d,%f,%f\n", M, size, seq_time, par_time);  // Scrittura dei tempi nel file
		fclose(f);  // Chiusura del file
	}

	MPI_Finalize();  // Terminazione di MPI
	return 0;
}
