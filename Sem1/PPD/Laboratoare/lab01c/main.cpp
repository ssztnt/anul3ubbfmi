#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

int generateRandomNumber(int upperBoundary) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, upperBoundary);
    return dist(gen);
}

void printArray(const vector<int>& a, int n) {
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << "\n";
}

// Sequential op used by the sequential task
inline int op(int a, int b) { return a + b; }

// Sequential version (for baseline timing)
void task_seq(const vector<int>& a, const vector<int>& b, vector<int>& c, int n) {
    for (int i = 0; i < n; i++)
        c[i] = op(a[i], b[i]);
}

// Parallel worker: computes c[i] = a[i] + b[i] for i in [start, end)
void task_range(const vector<int>& a, const vector<int>& b, vector<int>& c, int start, int end) {
    for (int i = start; i < end; ++i)
        c[i] = a[i] + b[i];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Hello, World!\n";

    int n = 10000000;
    int upperBound = 10;

    vector<int> a(n), b(n), c_seq(n), c_par(n);

    for (int i = 0; i < n; i++) {
        a[i] = generateRandomNumber(upperBound);
        b[i] = generateRandomNumber(upperBound);
    }

    // --- Sequential timing ---
    auto t_start = chrono::high_resolution_clock::now();
    task_seq(a, b, c_seq, n);
    auto t_end = chrono::high_resolution_clock::now();
    double elapsed_seq_ms = chrono::duration<double, milli>(t_end - t_start).count();

    if (n < 10) {
        printArray(a, n);
        printArray(b, n);
        printArray(c_seq, n);
    }

    cout << "Sequential time (ms): " << elapsed_seq_ms << "\n";

    // --- Parallel timing ---
    // Pick number of threads: use up to 4 or the number of elements, whichever is smaller
    unsigned hw = thread::hardware_concurrency();
    int p = 4;
    if (hw != 0) p = min<int>(p, hw);
    p = max(1, min(p, n)); // don’t spawn more threads than elements, at least 1

    auto t_start2 = chrono::high_resolution_clock::now();

    vector<thread> threads;
    threads.reserve(p);

    int base = n / p;
    int rem = n % p;

    int start = 0;
    for (int i = 0; i < p; ++i) {
        int len = base + (i < rem ? 1 : 0);
        int end = start + len;
        if (len > 0) {
            threads.emplace_back(task_range, cref(a), cref(b), ref(c_par), start, end);
        }
        start = end;
    }

    for (auto& th : threads) th.join();

    auto t_end2 = chrono::high_resolution_clock::now();
    double elapsed_par_ms = chrono::duration<double, milli>(t_end2 - t_start2).count();

    // Verify correctness
    bool ok = equal(c_seq.begin(), c_seq.end(), c_par.begin());
    if (!ok) {
        cerr << "Mismatch between sequential and parallel results!\n";
        return 1;
    }

    if (n < 10) {
        printArray(c_par, n);
    }

    cout << "Threads used: " << p << "\n";
    cout << "Parallel time (ms): " << elapsed_par_ms << "\n";

    return 0;
}