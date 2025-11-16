#include <mpi.h>
#include <iostream>

#include "header/GenerateNumber.h"
#include "header/ScatterCalculation.h"
#include "header/SequentialCalculation.h"
#include "header/StandardCalculation.h"
#include "header/AsyncCalculation.h"
#include "header/OptimizedCalculation.h"
#include "header/Verification.h"

using namespace std;

void printMenu() {
    cout << "\n========================================" << endl;
    cout << "  MPI LARGE NUMBER ADDITION" << endl;
    cout << "========================================" << endl;
    cout << "Select variant to run:" << endl;
    cout << "  0 - Sequential (reference)" << endl;
    cout << "  1 - Variant 1: Standard Communication (MPI_Send/MPI_Recv)" << endl;
    cout << "  2 - Variant 2: Scatter/Gather" << endl;
    cout << "  3 - Variant 3: Asynchronous Communication" << endl;
    cout << "  4 - Variant 1.1: Optimized (addition before carry)" << endl;
    cout << "  5 - Run ALL variants + verification" << endl;
    cout << "  6 - Verification only (compare existing results)" << endl;
    cout << "========================================" << endl;
    cout << "Choice: ";
}

void printOutputInfo(int choice) {
    cout << "\n========== OUTPUT FILE INFO ==========" << endl;
    switch(choice) {
        case 0:
            cout << "Output: result.txt" << endl;
            cout << "Description: Sequential calculation (reference)" << endl;
            break;
        case 1:
            cout << "Output: result1.txt" << endl;
            cout << "Description: Variant 1 - Standard MPI communication" << endl;
            break;
        case 2:
            cout << "Output: resultScatter.txt" << endl;
            cout << "Description: Variant 2 - Scatter/Gather collective operations" << endl;
            break;
        case 3:
            cout << "Output: resultAsync.txt" << endl;
            cout << "Description: Variant 3 - Asynchronous communication (MPI_Isend/MPI_Irecv)" << endl;
            break;
        case 4:
            cout << "Output: resultOptimized.txt" << endl;
            cout << "Description: Variant 1.1 - Optimized (processes start adding before receiving carry)" << endl;
            break;
        case 5:
            cout << "Outputs:" << endl;
            cout << "  - result.txt (Sequential)" << endl;
            cout << "  - result1.txt (Variant 1)" << endl;
            cout << "  - resultScatter.txt (Variant 2)" << endl;
            cout << "  - resultAsync.txt (Variant 3)" << endl;
            cout << "  - resultOptimized.txt (Variant 1.1)" << endl;
            break;
    }
    cout << "========================================\n" << endl;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <N1> <N2> [variant]" << endl;
        cerr << "  N1: Number of digits in first number" << endl;
        cerr << "  N2: Number of digits in second number" << endl;
        cerr << "  variant: Optional (0-6), if not provided, shows menu" << endl;
        return 1;
    }
    
    int N1 = atoi(argv[1]);
    int N2 = atoi(argv[2]);
    int choice = -1;

    int N_MAX;
    if (N1 > N2) {
        N_MAX = N1;
    } else {
        N_MAX = N2;
    }

    // Initialize MPI first
    MPI_Init(&argc, &argv);
    int P, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Only process 0 handles menu and generates numbers
    if (rank == 0) {
        if (argc >= 4) {
            choice = atoi(argv[3]);
        } else {
            printMenu();
            cin >> choice;
        }

        if (choice < 0 || choice > 6) {
            cout << "Invalid choice! Running all variants..." << endl;
            choice = 5;
        }

        printOutputInfo(choice);

        // Generate numbers
        GenerateNumber::generateNumber("firstNumber.txt", N1);
        GenerateNumber::generateNumber("secondNumber.txt", N2);
    }

    // Broadcast choice to all processes
    MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Ensure all processes wait for file generation
    MPI_Barrier(MPI_COMM_WORLD);

    // Run sequential first (only process 0)
    if (rank == 0 && choice != 6) {
        int *firstNumber = GenerateNumber::readNumber("firstNumber.txt");
        int *secondNumber = GenerateNumber::readNumber("secondNumber.txt");

        SequentialCalculation calculation(firstNumber, secondNumber, N1, N2);
        int *number = calculation.calculate();
        GenerateNumber::writeNumber("result.txt", number, N_MAX);
        
        delete[] firstNumber;
        delete[] secondNumber;
        delete[] number;

        if (choice == 0) {
            cout << "✓ Sequential calculation completed" << endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Run selected variant(s)
    switch(choice) {
        case 1: {
            StandardCalculation calculator(P, N_MAX);
            calculator.run();
            if (rank == 0) cout << "✓ Variant 1 (Standard) completed" << endl;
            break;
        }
        case 2: {
            ScatterCalculation calculatorS(P, N_MAX);
            calculatorS.run();
            if (rank == 0) cout << "✓ Variant 2 (Scatter/Gather) completed" << endl;
            break;
        }
        case 3: {
            AsyncCalculation calculatorA(P, N_MAX);
            calculatorA.run();
            if (rank == 0) cout << "✓ Variant 3 (Async) completed" << endl;
            break;
        }
        case 4: {
            OptimizedCalculation calculatorOpt(P, N_MAX);
            calculatorOpt.run();
            if (rank == 0) cout << "✓ Variant 1.1 (Optimized) completed" << endl;
            break;
        }
        case 5: {
            // Run all variants
            StandardCalculation calculator(P, N_MAX);
            calculator.run();
            if (rank == 0) cout << "✓ Variant 1 completed" << endl;

            ScatterCalculation calculatorS(P, N_MAX);
            calculatorS.run();
            if (rank == 0) cout << "✓ Variant 2 completed" << endl;

            AsyncCalculation calculatorA(P, N_MAX);
            calculatorA.run();
            if (rank == 0) cout << "✓ Variant 3 completed" << endl;

            OptimizedCalculation calculatorOpt(P, N_MAX);
            calculatorOpt.run();
            if (rank == 0) cout << "✓ Variant 1.1 completed" << endl;
            break;
        }
        case 6:
            // Verification only - do nothing
            break;
        default:
            if (rank == 0) cout << "No variant executed for choice: " << choice << endl;
    }

    MPI_Finalize();

    // Verify results (only process 0)
    if (rank == 0 && (choice == 5 || choice == 6)) {
        Verification::runAllVerifications();
    }

    return 0;
}