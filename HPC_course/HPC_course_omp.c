/* Compute pi in OMP */
#include "lcgenerator.h"
#include <stdio.h>
static long num_trials = 1000000;

int main(int argc, char** argv) {
    long i;
    long Ncirc = 0;
    double pi, x, y;
    double r = 1.0; // radius of circle
    double r2 = r * r;

#pragma omp parallel default(none) private(x, y, i) shared(Ncirc, r2, num_trials)
#pragma omp for reduction (+ : Ncirc)  
    for (i = 0; i < num_trials; i++) {
#pragma omp critical
{
        x = lcgrandom();
        y = lcgrandom();
}
        if ((x * x + y * y) <= r2)
            Ncirc++;
    }

    // Here we are serial again
    pi = 4.0 * ((double)Ncirc) / ((double)num_trials);

    printf("\n \t Computing pi in OMP: \n");
    printf("\t For %ld trials, pi = %f\n", num_trials, pi);
    printf("\n");

    return 0;
}
