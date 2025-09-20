#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10   // tamanho do vetor
#define MAX 100 // limite superior dos valores aleatórios

int main(int argc, char* argv[]) {
    int rank, numProcs;
    int maior_parcial, maior_final;
    int buffer[N], subvetor[N]; // subvetor vai receber pedaço do scatter
    int tam_subvetor;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // tamanho do pedaço que cada processo vai receber
    tam_subvetor = N / numProcs;

    if (rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            buffer[i] = rand() % MAX;
            printf("buffer[%d] = %d\n", i, buffer[i]);
        }
    }

    // distribuir o vetor para todos os processos
    MPI_Scatter(buffer, tam_subvetor, MPI_INT,
                subvetor, tam_subvetor, MPI_INT,
                0, MPI_COMM_WORLD);

    // cada processo calcula o maior parcial
    maior_parcial = subvetor[0];
    for (int i = 1; i < tam_subvetor; i++) {
        if (subvetor[i] > maior_parcial) {
            maior_parcial = subvetor[i];
        }
    }

    // reduzir todos os maiores parciais no processo 0 (com operação MPI_MAX)
    MPI_Reduce(&maior_parcial, &maior_final, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nMaior elemento do vetor = %d\n", maior_final);
    }

    MPI_Finalize();
    return 0;
}
