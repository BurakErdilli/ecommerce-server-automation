#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Function prototypes 

/*Trial Division: 180921733
Sieve of Eratosthenes: 182171
Optimized Sieve: 283453
Miller-Rabin: 294271253
Fermat's Little Theorem: 48991
*/

int trial_division(int n);
int sieve_of_eratosthenes(int limit);
int sieve_of_eratosthenes_optimized(int limit);
int miller_rabin(int n, int k);
int fermat_test(int n, int k);
int miller_rabin_wrapper(int n);
int fermat_test_wrapper(int n);
double get_time_in_seconds();
int find_max_prime_in_1_second(int (*prime_func)(int));

// Define for cross-platform time support
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define TIME_LIMIT 50.0 // Time limit in seconds

// Trial Division
int trial_division(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1; // 2 and 3 are primes
    if (n % 2 == 0 || n % 3 == 0) return 0;

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }

    return 1; // n is prime
}

// Sieve of Eratosthenes
int sieve_of_eratosthenes(int limit) {
    int *sieve = (int *)calloc(limit + 1, sizeof(int));
    for (int i = 2; i <= limit; i++) {
        sieve[i] = 1;
    }
    for (int i = 2; i * i <= limit; i++) {
        if (sieve[i]) {
            for (int j = i * i; j <= limit; j += i) {
                sieve[j] = 0;
            }
        }
    }

    int max_prime = 2;
    for (int i = limit; i >= 2; i--) {
        if (sieve[i]) {
            max_prime = i;
            break;
        }
    }
    free(sieve);
    return max_prime;
}

// Optimized Sieve of Eratosthenes
int sieve_of_eratosthenes_optimized(int limit) {
    int *sieve = (int *)calloc((limit / 2) + 1, sizeof(int));
    for (int i = 1; i <= (limit / 2); i++) {
        sieve[i] = 1;
    }

    for (int i = 3; i * i <= limit; i += 2) {
        if (sieve[i / 2]) {
            for (int j = i * i; j <= limit; j += 2 * i) {
                sieve[j / 2] = 0;
            }
        }
    }

    int max_prime = 2;
    for (int i = (limit / 2); i >= 1; i--) {
        if (sieve[i]) {
            max_prime = 2 * i + 1;
            break;
        }
    }
    free(sieve);
    return max_prime;
}

// Miller-Rabin Primality Test
int miller_rabin(int n, int k) {
    if (n == 2 || n == 3) return 1;
    if (n <= 1 || n % 2 == 0) return 0;

    int d = n - 1, r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    for (int i = 0; i < k; i++) {
        int a = 2 + rand() % (n - 3);
        long long x = 1, base = a;
        for (int exp = d; exp > 0; exp /= 2) {
            if (exp % 2 == 1) x = (x * base) % n;
            base = (base * base) % n;
        }
        if (x == 1 || x == n - 1) continue;

        int composite = 1;
        for (int j = 1; j < r; j++) {
            x = (x * x) % n;
            if (x == n - 1) {
                composite = 0;
                break;
            }
        }
        if (composite) return 0;
    }
    return 1;
}

// Fermat's Little Theorem
int fermat_test(int n, int k) {
    if (n == 2) return 1; // 2 is prime
    if (n <= 1 || n % 2 == 0) return 0; // Exclude non-prime cases

    for (int i = 0; i < k; i++) {
        int a = 2 + rand() % (n - 2); // Random base `a` in the range [2, n-2]
        int result = 1;
        int base = a;
        int exp = n - 1;

        // Modular Exponentiation: (a^(n-1)) % n
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base) % n;
            }
            base = (base * base) % n;
            exp /= 2;
        }

        if (result != 1) return 0; // Fermat's theorem failed
    }
    return 1; // Passed all tests
}

// Wrapper for Miller-Rabin Test
int miller_rabin_wrapper(int n) {
    return miller_rabin(n, 5); // Fix 'k' as 5 (number of iterations)
}

// Wrapper for Fermat Test
int fermat_test_wrapper(int n) {
    return fermat_test(n, 5); // Fix 'k' as 5
}

// Cross-platform time function for timing
#ifdef _WIN32
double get_time_in_seconds() {
    FILETIME ft;
    LARGE_INTEGER li;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    return (double)li.QuadPart / 10000000.0 - 11644473600.0;
}
#else
double get_time_in_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}
#endif

// Find max prime in a given time limit
int find_max_prime_in_1_second(int (*prime_func)(int)) {
    double start_time = get_time_in_seconds();

    int max_prime = -1;
    int candidate = 2;

    while (1) {
        if (prime_func(candidate)) {
            max_prime = candidate;
        }

        candidate++;

        double current_time = get_time_in_seconds();
        if (current_time - start_time >= TIME_LIMIT) {
            break;
        }
    }

    return max_prime;
}

int main() {
    printf("Trial Division: %d\n", find_max_prime_in_1_second(trial_division));
    printf("Sieve of Eratosthenes: %d\n", find_max_prime_in_1_second(sieve_of_eratosthenes));
    printf("Optimized Sieve: %d\n", find_max_prime_in_1_second(sieve_of_eratosthenes_optimized));
    printf("Miller-Rabin: %d\n", find_max_prime_in_1_second(miller_rabin_wrapper));
    printf("Fermat's Little Theorem: %d\n", find_max_prime_in_1_second(fermat_test_wrapper));

    return 0;
}
