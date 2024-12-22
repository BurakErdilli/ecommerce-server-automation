#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function prototypes
int fib_rec(int n);
int fib_iterative(int n);
int fib_hashing(int n, int *memo);
int fib_brave(int n);
int fib_matrix_advanced(int n);
int fib_improved_math(int n);

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

// Memoization using a hash-like array (dynamic programming)
int fib_hashing(int n, int *memo) {
  if (n <= 1) {
    return n;
  }
  if (memo[n] != -1) {
    return memo[n];
  }
  memo[n] = fib_hashing(n - 1, memo) + fib_hashing(n - 2, memo);
  return memo[n];
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

// Main function
int main() {
  int n;
  clock_t start, end;

  printf("Enter the number of terms: ");
  scanf("%d", &n);

  // Recursive
  printf("Fibonacci series using recursive function:\n");
  start = clock();
  for (int i = 0; i < n; i++) {
    printf("%d ", fib_rec(i));
  }
  end = clock();
  printf("\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  // Iterative
  printf("Fibonacci series using iterative function:\n");
  start = clock();
  for (int i = 0; i < n; i++) {
    printf("%d ", fib_iterative(i));
  }
  end = clock();
  printf("\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  // Hashing (memoization)
  printf("Fibonacci series using hashing (memoization):\n");
  start = clock();
  int *memo = (int *)malloc((n + 1) * sizeof(int));
  for (int i = 0; i <= n; i++) {
    memo[i] = -1; // Initialize the memoization array
  }
  for (int i = 0; i < n; i++) {
    printf("%d ", fib_hashing(i, memo));
  }
  free(memo);
  end = clock();
  printf("\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  // Brave algorithm
  printf("Fibonacci series using Brave algorithm:\n");
  start = clock();
  for (int i = 0; i < n; i++) {
    printf("%d ", fib_brave(i));
  }
  end = clock();
  printf("\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  // Matrix exponentiation
  printf("Fibonacci series using matrix exponentiation (advanced):\n");
  start = clock();
  for (int i = 0; i < n; i++) {
    printf("%d ", fib_matrix_advanced(i));
  }
  end = clock();
  printf("\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  // Improved mathematical approximation
  printf("Fibonacci series using improved math (sine-based):\n");
  start = clock();
  for (int i = 0; i < n; i++) {
    printf("%d ", fib_improved_math(i));
  }
  end = clock();
  printf("\nTime taken: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

  return 0;
}
