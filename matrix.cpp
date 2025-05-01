#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    int n;

    cout << "Enter the size of the square matrices (n x n): ";
    cin >> n;

    // Declare matrices
    int A[100][100], B[100][100], C[100][100];

    // Input Matrix A
    cout << "Enter elements of Matrix A:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A[i][j];

    // Input Matrix B
    cout << "Enter elements of Matrix B:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> B[i][j];

    // Initialize result matrix C
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = 0;

    // Parallel matrix multiplication
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
                int thread_id = omp_get_thread_num();
               int total_threads = omp_get_num_threads();
               cout << "Thread " << thread_id << "/" << total_threads << " computing C[" << i << "][" << j << "]\n";
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Print Result
    cout << "\nResult Matrix C (A x B):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << C[i][j] << " ";
        cout << endl;
    }

    return 0;
}
