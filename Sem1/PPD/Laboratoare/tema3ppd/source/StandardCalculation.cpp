#include "../header/StandardCalculation.h"
#include "mpi.h"
#include "../header/GenerateNumber.h"
#include <fstream>

using namespace std;

void StandardCalculation::run() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    calculator(rank);
}

void StandardCalculation::calculator(int rank) {
if (rank == 0) {
        ofstream out("result1.txt");
        const int dimension = N_Max / (P - 1);
        int extra = N_Max % (P - 1);
        int startPoint = 0;

        for (int pid = 1; pid < P; pid++) {
            int endPoint = startPoint + dimension;
            if (extra > 0) {
                endPoint++;
                extra--;
            }
            int batchSize = endPoint - startPoint;

            int *firstNumber = GenerateNumber::readNumberBlock("firstNumber.txt", startPoint, batchSize);
            int *secondNumber = GenerateNumber::readNumberBlock("secondNumber.txt", startPoint, batchSize);

            MPI_Send(firstNumber, batchSize, MPI_INT, pid, 0, MPI_COMM_WORLD);
            MPI_Send(secondNumber, batchSize, MPI_INT, pid, 1, MPI_COMM_WORLD);

            startPoint = endPoint;

             delete[] firstNumber;
             delete[] secondNumber;
        }


        extra = N_Max % (P - 1);
        startPoint = 0;

        for (int pid = 1; pid < P; pid++) {
            int endPoint = startPoint + dimension;
            if (extra > 0) {
                endPoint++;
                extra--;
            }
            int batchSize = endPoint - startPoint;

            int *result = new int[batchSize];
            MPI_Recv(result, batchSize, MPI_INT, pid, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < batchSize; i++) {
                out << result[i] << " ";
            }


            if (pid == P - 1) {
                int flag;
                MPI_Recv(&flag, 1, MPI_INT, pid, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (flag != 0) {
                    out << flag;
                }
            }
            startPoint = endPoint;
            delete[] result;
        }
        out.close();
    } else {
        const int dimension = N_Max / (P - 1);
        const int extra = N_Max % (P - 1);
        const int batchSize = dimension + ((rank - 1) < extra);

        int *firstNumber = new int[batchSize];
        int *secondNumber = new int[batchSize];

        MPI_Recv(firstNumber, batchSize,MPI_INT, 0, 0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(secondNumber, batchSize,MPI_INT, 0, 1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        int *result = new int[batchSize];


        int carry = sum(firstNumber, secondNumber, result, batchSize);


        if (rank > 1) {
            int receivedCarry;
            MPI_Recv(&receivedCarry, 1,MPI_INT, rank - 1, 4,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            if (receivedCarry > 0) {
                passCarry(result, batchSize, receivedCarry);
                carry+=receivedCarry;
            }
        }

        if (rank < (P - 1)) {
            MPI_Send(&carry, 1,MPI_INT, rank + 1, 4,MPI_COMM_WORLD);
        }

        MPI_Send(result, batchSize,MPI_INT, 0, 2,MPI_COMM_WORLD);
        if (rank == P - 1) {
            MPI_Send(&carry, 1,MPI_INT, 0, 3,MPI_COMM_WORLD);
        }

        delete[] firstNumber;
        delete[] secondNumber;
        delete[] result;
    }
}

int StandardCalculation::sum(const int *firstNumber, const int *secondNumber, int *result, const int size) {
    int carry = 0;
    for (int i = 0; i < size; i++) {
        result[i] = (firstNumber[i] + secondNumber[i] + carry) % 10;
        carry = (firstNumber[i] + secondNumber[i] + carry) / 10;
    }
    return carry;
}

void StandardCalculation::passCarry(int *number, const int size, int &carry) {
    for (int i = 0; i < size; i++) {
        const int value = number[i] + carry;
        number[i] = value % 10;
        carry = value / 10;
    }
}
