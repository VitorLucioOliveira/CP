/*
* Adapted from: http://w...content-available-to-author-only...s.org/sieve-of-eratosthenes
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int sieveOfEratosthenes(int n)
{
   // Create a boolean array "prime[0..n]" and initialize
   // all entries it as true. A value in prime[i] will
   // finally be false if i is Not a prime, else true.
   int primes = 0; 
   bool *prime = (bool*) malloc((n+1)*sizeof(bool));
   int sqrt_n = sqrt(n);

   memset(prime, true,(n+1)*sizeof(bool));

   #pragma omp parallel for schedule(static, 4)
   for (int p=2; p <= sqrt_n; p++)
   {
       // If prime[p] is not changed, then it is a prime
       if (prime[p] == true)
       {
           // Update all multiples of p
           for (int i=p*2; i<=n; i += p)
           prime[i] = false;
        }
    }

    // count prime numbers
    #pragma omp parallel for reduction(+:primes)
    for (int p=2; p<=n; p++)
       if (prime[p])
         primes++;

    return(primes);
}

int main()
{
   int n = 100000000;
   printf("%d\n",sieveOfEratosthenes(n));
   return 0;
} 

// Tempos
//------------------
// Sequencial = 3.026s
// Paralelo (schedlue dynamic) = 1,924s --> Speedup = 1.57
//             '--> chunck (4) = 2,368s --> Speedup = 1.27
// Paralelo (schedlue static)  = 2,908s --> Speedup = 1.04
//             '--> chunck (4) = 2,090s --> Speedup = 1.44
// Paralelo (schedlue guided ) = 2,855s --> Speedup = 1.05
//             '--> chunck (4) = 3,026s --> Speedup = 1.00


// Arquitetura do meu notebook
//-------------------------------
//     Architecture:             x86_64
//     CPU op-mode(s):         32-bit, 64-bit
//     Address sizes:          36 bits physical, 48 bits virtual
//     Byte Order:             Little Endian
//     CPU(s):                   4
//     On-line CPU(s) list:    0-3
//     Vendor ID:                GenuineIntel
//     Model name:             Intel(R) Core(TM) i5 CPU       M 460  @ 2.53GHz
//        CPU family:           6
//        Model:                37
//        Thread(s) per core:   2

