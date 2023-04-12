/* Compute pi in parallel */
#include <mpi.h>
#include "lcgenerator.h"
#include <stdio.h>
static long num_trials = 1000000;

int main(int argc, char** argv) {
    long i;
    long Ncirc = 0;
    double pi, x, y;
    double r = 1.0; // radius of circle
    double r2 = r * r;
    int size, rank;
    long my_num_trials;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    my_num_trials = num_trials / size;
    if (rank < (num_trials % size))
        my_num_trials++;

    random_last = rank; //changes the randomly generated values in every process. random_last is a global variable from lcgenerator.h

    for (i = 0; i < my_num_trials; i++) {
        x = lcgrandom();
        y = lcgrandom();
        if ((x * x + y * y) <= r2)
            Ncirc++;
    }

    if (rank == 0)
    {
        // receive from the other processors
        // compute pi and print it out
        int i, total = Ncirc;
        for (i = 1; i < size; i++)
        {
            //MPI_Recv(&Ncirc, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status);
            MPI_Recv(&Ncirc, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            total += Ncirc;
        }

        pi = 4.0 * ((double)total) / ((double)num_trials);

        printf("\n \t Computing pi in parallel: \n");
        printf("\t For %ld trials, pi = %f\n", num_trials, pi);
        printf("\n");
    }
    else
    {
        MPI_Send(&Ncirc, 1, MPI_LONG, 0, rank, MPI_COMM_WORLD);
    }


    MPI_Finalize();
    return 0;
}
