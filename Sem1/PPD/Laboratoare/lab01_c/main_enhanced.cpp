/**
 * Laborator 1: Aplicarea unei matrice de convolutie
 * 
 * Multe dintre filtrele pe imagini utilizeaza operatia de convolutie bazata pe matrice de convolutie.
 * Simplu spus, convolutia este tratamentul aplicat unei matrice F de dimensiune NxM (care poate fi
 * o matrice care contine cate un pixel) prin intermediul altei matrice numita matricea de convolutie - C.
 * 
 * Prin aplicarea convolutiei se modifica fiecare element al matricei date initial. Putem sa consideram
 * si cazul in care rezultatul convolutiei se pastreaza intr-o matrice noua V de aceeasi dimensiune
 * ca si matricea initiala (n,m).
 * 
 * Exemplu: Pentru matrice de convolutie C de dimensiune 3x3 (k=3) aplicat unei matrice date F de
 * dimensiune 5x5 (m=5, n=5)
 */

#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <iomanip>

#define LIMIT 10
#define THREAD_MODE "rows"
using namespace std;

int rows, cols;
int convolusion_rows, convolusion_cols;
int no_threads;
int result[10000][10000];
int matrix[10000][10000], convolusion[5][10000];
double final_time;
bool read_from_file = false;

/**
 * Thread class pentru procesarea pe linii (rows)
 * Fiecare thread proceseaza un subset de linii din matrice
 */
class LineThread {
    int start, stop;

public:
    LineThread(int start, int stop) : start(start), stop(stop) {}

    void operator()() const {
        // Pentru fiecare linie din range-ul alocat
        for (int i = start; i < stop; i++) {
            for (int j = 0; j < cols; j++) {
                // Aplicam convolutia: iteram prin kernel-ul de convolutie
                for (int convolusion_i = i - convolusion_rows / 2; 
                     convolusion_i <= i + convolusion_rows / 2; 
                     convolusion_i++) {
                    
                    // Edge handling: clamp la marginile matricei
                    int final_i;
                    if (convolusion_i < 0) 
                        final_i = 0;
                    else if (convolusion_i >= rows) 
                        final_i = rows - 1;
                    else 
                        final_i = convolusion_i;

                    for (int convolusion_j = j - convolusion_cols / 2; 
                         convolusion_j <= j + convolusion_cols / 2;
                         convolusion_j++) {
                        
                        int final_j;
                        if (convolusion_j < 0) 
                            final_j = 0;
                        else if (convolusion_j >= cols) 
                            final_j = cols - 1;
                        else 
                            final_j = convolusion_j;

                        // Calculam produsul si il adunam la rezultat
                        result[i][j] += matrix[final_i][final_j] * 
                                       convolusion[convolusion_i - i + convolusion_rows / 2]
                                                  [convolusion_j - j + convolusion_cols / 2];
                    }
                }
            }
        }
    }
};

/**
 * Thread class pentru procesarea pe coloane (columns)
 * Fiecare thread proceseaza un subset de coloane din matrice
 */
class ColumnThread {
    int start, stop;

public:
    ColumnThread(int start, int stop) : start(start), stop(stop) {}

    void operator()() const {
        for (int i = 0; i < rows; i++) {
            for (int j = start; j < stop; j++) {
                for (int convolusion_i = i - convolusion_rows / 2; 
                     convolusion_i <= i + convolusion_rows / 2;
                     convolusion_i++) {
                    
                    int final_i;
                    if (convolusion_i < 0) 
                        final_i = 0;
                    else if (convolusion_i >= rows) 
                        final_i = rows - 1;
                    else 
                        final_i = convolusion_i;

                    for (int convolusion_j = j - convolusion_cols / 2; 
                         convolusion_j <= j + convolusion_cols / 2;
                         convolusion_j++) {
                        
                        int final_j;
                        if (convolusion_j < 0) 
                            final_j = 0;
                        else if (convolusion_j >= cols) 
                            final_j = cols - 1;
                        else 
                            final_j = convolusion_j;

                        result[i][j] += matrix[final_i][final_j] * 
                                       convolusion[convolusion_i - i + convolusion_rows / 2]
                                                  [convolusion_j - j + convolusion_cols / 2];
                    }
                }
            }
        }
    }
};

/**
 * Genereaza matrice cu valori random (pentru teste de performanta)
 */
void generate_matrix_static(int rows, int cols, int limit, int ma[][10000]) {
    random_device random_device;
    mt19937 gen(random_device());
    uniform_int_distribution<int> distr(0, limit);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) 
            ma[i][j] = distr(gen);
}

/**
 * Citeste matrice din fisier
 */
void read_matrix_from_file(ifstream& f, int rows, int cols, int ma[][10000]) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            f >> ma[i][j];
}

/**
 * Varianta secventiala (fara thread-uri)
 */
void secvential() {
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int convolusion_i = i - convolusion_rows / 2; 
                 convolusion_i <= i + convolusion_rows / 2; 
                 convolusion_i++) {
                
                int final_i;
                if (convolusion_i < 0) 
                    final_i = 0;
                else if (convolusion_i >= rows) 
                    final_i = rows - 1;
                else 
                    final_i = convolusion_i;

                for (int convolusion_j = j - convolusion_cols / 2; 
                     convolusion_j <= j + convolusion_cols / 2;
                     convolusion_j++) {
                    
                    int final_j;
                    if (convolusion_j < 0) 
                        final_j = 0;
                    else if (convolusion_j >= cols) 
                        final_j = cols - 1;
                    else 
                        final_j = convolusion_j;

                    result[i][j] += matrix[final_i][final_j] * 
                                   convolusion[convolusion_i - i + convolusion_rows / 2]
                                              [convolusion_j - j + convolusion_cols / 2];
                }
            }
        }
    }
    
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    final_time = duration.count() / 1000.0;
}

/**
 * Varianta paralela pe linii (rows)
 */
void rows_thread_run() {
    auto start_time = chrono::high_resolution_clock::now();

    int start = 0;
    int end = rows / no_threads;
    int rest = rows % no_threads;
    int step = rows / no_threads;
    vector<thread> threads;

    for (int i = 0; i < no_threads; i++) {
        if (rest > 0) {
            end++;
            rest--;
        }

        threads.push_back(thread(LineThread(start, end)));
        start = end;
        end = start + step;
    }

    for (int i = 0; i < no_threads; i++)
        threads[i].join();
        
    auto stop_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop_time - start_time);
    final_time = duration.count() / 1000.0;
}

/**
 * Varianta paralela pe coloane (columns)
 */
void column_thread_run() {
    auto start_time = chrono::high_resolution_clock::now();

    int start = 0;
    int end = cols / no_threads;
    int rest = cols % no_threads;
    int step = cols / no_threads;
    vector<thread> threads;

    for (int i = 0; i < no_threads; i++) {
        if (rest > 0) {
            end++;
            rest--;
        }

        threads.push_back(thread(ColumnThread(start, end)));
        start = end;
        end = start + step;
    }

    for (int i = 0; i < no_threads; i++)
        threads[i].join();

    auto stop_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop_time - start_time);
    final_time = duration.count() / 1000.0;
}

/**
 * Afiseaza matricea de rezultat
 */
void print_result() {
    cout << "\n=== Matricea rezultat ===" << endl;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << setw(6) << result[i][j] << " ";
        }
        cout << endl;
    }
}

/**
 * Afiseaza matricea originala
 */
void print_matrix() {
    cout << "\n=== Matricea originala ===" << endl;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << setw(6) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

/**
 * Afiseaza kernel-ul de convolutie
 */
void print_kernel() {
    cout << "\n=== Kernel de convolutie ===" << endl;
    for(int i = 0; i < convolusion_rows; i++) {
        for(int j = 0; j < convolusion_cols; j++) {
            cout << setw(6) << convolusion[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <num_threads> [input_file]" << endl;
        cout << "  num_threads: 0 pentru secvential, >0 pentru paralel" << endl;
        cout << "  input_file: optional, fisier cu matrici (altfel genereaza random)" << endl;
        return 1;
    }

    no_threads = atoi(argv[1]);
    string input_file = "input.txt";
    
    if (argc >= 3) {
        input_file = argv[2];
        read_from_file = true;
    }

    ifstream f(input_file);
    if (!f.is_open()) {
        cout << "Eroare: Nu pot deschide fisierul " << input_file << endl;
        return 1;
    }

    f >> rows >> cols;
    f >> convolusion_rows >> convolusion_cols;

    cout << "=== Configuratie ===" << endl;
    cout << "Dimensiune matrice: " << rows << "x" << cols << endl;
    cout << "Dimensiune kernel: " << convolusion_rows << "x" << convolusion_cols << endl;
    cout << "Numar thread-uri: " << (no_threads == 0 ? "secvential" : to_string(no_threads)) << endl;
    cout << "Mod: " << THREAD_MODE << endl;

    // Initializare matrice rezultat cu 0
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i][j] = 0;

    if (read_from_file || f.peek() != EOF) {
        // Citeste matricile din fisier daca exista date
        read_matrix_from_file(f, rows, cols, matrix);
        read_matrix_from_file(f, convolusion_rows, convolusion_cols, convolusion);
        cout << "Matrici citite din fisier." << endl;
        
        // Afiseaza matricile pentru verificare
        print_matrix();
        print_kernel();
    } else {
        // Genereaza matrici random
        generate_matrix_static(rows, cols, LIMIT, matrix);
        generate_matrix_static(convolusion_rows, convolusion_cols, LIMIT, convolusion);
        cout << "Matrici generate random (0-" << LIMIT << ")." << endl;
    }
    f.close();

    // Ruleaza convolutia
    if (no_threads == 0) {
        secvential();
    } else {
        if (string(THREAD_MODE) == "rows")
            rows_thread_run();
        else
            column_thread_run();
    }

    // Afiseaza rezultatul
    print_result();
    
    cout << "\n=== Timp executie ===" << endl;
    cout << "Timp: " << final_time << " ms" << endl;

    return 0;
}

