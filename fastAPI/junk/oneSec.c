#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototypes
int fib_rec(int n);
int fib_iterative(int n);
int fib_hashing_wrapper(int n);
int fib_brave(int n);
int fib_matrix_advanced(int n);
int fib_improved_math(int n);

// Global memoization array for `fib_hashing`
#define MEMO_SIZE 1000000
int *memo;

// Recursive Fibonacci function
int fib_rec(int n) {
  if (n <= 1) {
    return n;
  }
  return fib_rec(n - 1) + fib_rec(n - 2);
}

// Iterative Fibonacci function
int fib_iterative(int n) {
  if (n <= 1) {
    return n;
  }
  int a = 0, b = 1, c;
  for (int i = 2; i <= n; i++) {
    c = a + b;
    a = b;
    b = c;
  }
  return b;
}

// Hashing (Memoization) function
int fib_hashing(int n) {
  if (n <= 1) {
    return n;
  }
  if (memo[n] != -1) {
    return memo[n];
  }
  memo[n] = fib_hashing(n - 1) + fib_hashing(n - 2);
  return memo[n];
}

// Wrapper for fib_hashing
int fib_hashing_wrapper(int n) {
  return fib_hashing(n);
}

// Brave algorithm (fast doubling method)
int fib_brave(int n) {
  if (n <= 1) {
    return n;
  }
  int a = 0, b = 1, c, d;
  for (int i = 31; i >= 0; i--) {
    c = a * (2 * b - a);    // c = F(2k)
    d = a * a + b * b;      // d = F(2k+1)
    if ((n >> i) & 1) {     // If n is odd
      a = d;
      b = c + d;
    } else {                // If n is even
      a = c;
      b = d;
    }
  }
  return a;
}

// Matrix exponentiation method
void multiply(int F[2][2], int M[2][2]) {
  int x = F[0][0] * M[0][0] + F[0][1] * M[1][0];
  int y = F[0][0] * M[0][1] + F[0][1] * M[1][1];
  int z = F[1][0] * M[0][0] + F[1][1] * M[1][0];
  int w = F[1][0] * M[0][1] + F[1][1] * M[1][1];
  F[0][0] = x;
  F[0][1] = y;
  F[1][0] = z;
  F[1][1] = w;
}

void power(int F[2][2], int n) {
  if (n == 0 || n == 1) {
    return;
  }
  int M[2][2] = {{1, 1}, {1, 0}};
  power(F, n / 2);
  multiply(F, F);
  if (n % 2 != 0) {
    multiply(F, M);
  }
}

int fib_matrix_advanced(int n) {
  if (n == 0) {
    return 0;
  }
  int F[2][2] = {{1, 1}, {1, 0}};
  power(F, n - 1);
  return F[0][0];
}

// Improved mathematical approach (using sine-based approximation)
int fib_improved_math(int n) {
  const double sqrt5 = sqrt(5);
  const double phi = (1 + sqrt5) / 2; // Golden ratio
  const double psi = (1 - sqrt5) / 2; // Conjugate of the golden ratio
  return (pow(phi, n) - pow(psi, n)) / sqrt5;
}

// Helper function to calculate max Fibonacci number in 1 second
int max_fib_in_1_second(int (*fib_func)(int)) {
  clock_t start, end;
  int n = 0, result = 0;
  start = clock();
  while (1) {
    result = fib_func(n);
    end = clock();
    if ((double)(end - start) / CLOCKS_PER_SEC >= 5.0) {
      break;
    }
    n++;
  }
  return n - 1; // Return the highest computed Fibonacci index
}

int main() {
  // Allocate and initialize memo array for hashing
  memo = (int *)malloc(MEMO_SIZE * sizeof(int));
  for (int i = 0; i < MEMO_SIZE; i++) {
    memo[i] = -1;
  }

  // Recursive
  printf("Recursive Fibonacci:\n");
  printf("Max Fibonacci index in 1 second: %d\n", max_fib_in_1_second(fib_rec));

  // Iterative
  printf("Iterative Fibonacci:\n");
  printf("Max Fibonacci index in 1 second: %d\n", max_fib_in_1_second(fib_iterative));

  /* Hashing
  printf("Hashing (Memoization) Fibonacci:\n");
  printf("Max Fibonacci index in 1 second: %d\n", max_fib_in_1_second(fib_hashing_wrapper));*/

  // Brave algorithm
  printf("Brave Algorithm Fibonacci:\n");
  printf("Max Fibonacci index in 1 second: %d\n", max_fib_in_1_second(fib_brave));

  // Matrix Exponentiation
  printf("Matrix Exponentiation Fibonacci:\n");
  printf("Max Fibonacci index in 1 second: %d\n", max_fib_in_1_second(fib_matrix_advanced));

  // Improved Math
  printf("Improved Math Fibonacci:\n");
  printf("Max Fibonacci index in 1 second: %d\n", max_fib_in_1_second(fib_improved_math));

  free(memo); // Free allocated memory
  return 0;
}
