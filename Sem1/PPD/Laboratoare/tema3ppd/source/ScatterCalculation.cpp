#include "../header/ScatterCalculation.h"
#include <mpi.h>
#include <fstream>

#include "../header/GenerateNumber.h"

void ScatterCalculation::run() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    calculator(rank);
}

void ScatterCalculation::calculator(int rank) {
    const int totalSize = N_Max % P == 0 ? N_Max : N_Max + (P - (N_Max % P));
    const int dimension = totalSize / P;
    int *firstNumber = nullptr;
    int *secondNumber = nullptr;
    int *result = nullptr;
    if (rank == 0) {
        firstNumber = GenerateNumber::readNumberP("firstNumber.txt", totalSize);
        secondNumber = GenerateNumber::readNumberP("secondNumber.txt", totalSize);
        result = new int[totalSize];
    }
    int *first_loc = new int[dimension];
    int *second_loc = new int[dimension];
    int *result_loc = new int[dimension];
    // se distribuie simultan numerele la fiecare proces
    MPI_Scatter(firstNumber, dimension, MPI_INT, first_loc, dimension, MPI_INT, 0, MPI_COMM_WORLD); // se distribuie simultan 
    MPI_Scatter(secondNumber, dimension, MPI_INT, second_loc, dimension, MPI_INT, 0, MPI_COMM_WORLD);

    int carry = sum(first_loc, second_loc, result_loc, dimension);// suma portiunii sale

    if (rank > 0) {
        int receivedCarry;
        MPI_Recv(&receivedCarry, 1,MPI_INT, rank - 1, 4,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        if (receivedCarry > 0) {
            passCarry(result_loc, dimension, receivedCarry);
            carry += receivedCarry; // fiecare proces caculeaza suma portiunii sale si o trimite la procesul urmator
        }
    }
    // worker trimite carry la procesul urmator
    if (rank < (P - 1)) {
        MPI_Send(&carry, 1,MPI_INT, rank + 1, 4,MPI_COMM_WORLD);
    } else if (rank == P - 1 && P > 1) { // daca este ultimul proces, trimite carry la master
        MPI_Send(&carry, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
    }
    // se colecteaza rezultatele la master
    MPI_Gather(result_loc, dimension, MPI_INT, result, dimension, MPI_INT, 0, MPI_COMM_WORLD);
    // se scrie rezultatul la master
    if (rank == 0) {
        ofstream outS("resultScatter.txt");
        for (int i = 0; i < N_Max; i++) {
            outS << result[i] << " ";
        }
        int final_carry = 0;

        MPI_Recv(&final_carry, 1, MPI_INT, P - 1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (final_carry > 0) {
            outS << final_carry << "\n";
        }
        outS.close();
        delete[] firstNumber;
        delete[] secondNumber;
        delete[] result;
    }
    delete[] first_loc;
    delete[] second_loc;
    delete[] result_loc;
}

int ScatterCalculation::sum(const int *firstNumber, const int *secondNumber, int *result, const int size) {
    int carry = 0;
    for (int i = 0; i < size; i++) {
        result[i] = (firstNumber[i] + secondNumber[i] + carry) % 10;
        carry = (firstNumber[i] + secondNumber[i] + carry) / 10;
    }
    return carry;
}

void ScatterCalculation::passCarry(int *number, const int size, int &carry) {
    for (int i = 0; i < size; i++) {
        const int value = number[i] + carry;
        number[i] = value % 10;
        carry = value / 10;
    }
}
