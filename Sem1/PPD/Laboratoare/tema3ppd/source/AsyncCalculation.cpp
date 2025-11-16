#include "../header/AsyncCalculation.h"
#include <fstream>
#include <mpi.h>
#include "../header/GenerateNumber.h"
using namespace std;

void AsyncCalculation::run() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    calculator(rank);
}

void AsyncCalculation::calculator(int rank) {
    if (rank == 0) {
        ofstream outA("resultAsync.txt");
        const int dimension = N_Max / (P - 1);
        int extra = N_Max % (P - 1);
        int startPoint = 0;
        
        // Arrays to keep track of send requests and data buffers
        MPI_Request* sendRequests = new MPI_Request[(P - 1) * 2];
        int** firstNumbers = new int*[P - 1];
        int** secondNumbers = new int*[P - 1];

        // Send data to all processes asynchronously
        for (int pid = 1; pid < P; pid++) {
            int endPoint = startPoint + dimension;
            if (extra > 0) {
                endPoint++;
                extra--;
            }
            int batchSize = endPoint - startPoint;
            
            firstNumbers[pid - 1] = GenerateNumber::readNumberBlock("firstNumber.txt", startPoint, batchSize);
            secondNumbers[pid - 1] = GenerateNumber::readNumberBlock("secondNumber.txt", startPoint, batchSize);
            
            MPI_Isend(firstNumbers[pid - 1], batchSize, MPI_INT, pid, 1, MPI_COMM_WORLD, &sendRequests[(pid - 1) * 2]);
            MPI_Isend(secondNumbers[pid - 1], batchSize, MPI_INT, pid, 2, MPI_COMM_WORLD, &sendRequests[(pid - 1) * 2 + 1]);
            
            startPoint = endPoint;
        }

        // Receive results from all processes asynchronously
        extra = N_Max % (P - 1);
        startPoint = 0;
        MPI_Request* recvRequests = new MPI_Request[P - 1];
        int** results = new int*[P - 1];
        int* batchSizes = new int[P - 1];

        for (int pid = 1; pid < P; pid++) {
            int endPoint = startPoint + dimension;
            if (extra > 0) {
                endPoint++;
                extra--;
            }
            batchSizes[pid - 1] = endPoint - startPoint;
            results[pid - 1] = new int[batchSizes[pid - 1]];
            
            MPI_Irecv(results[pid - 1], batchSizes[pid - 1], MPI_INT, pid, 3, MPI_COMM_WORLD, &recvRequests[pid - 1]);
            
            startPoint = endPoint;
        }

        // Wait for all receives to complete and write results
        for (int pid = 1; pid < P; pid++) {
            MPI_Wait(&recvRequests[pid - 1], MPI_STATUS_IGNORE);
            for (int i = 0; i < batchSizes[pid - 1]; i++) {
                outA << results[pid - 1][i] << " ";
            }
            
            // Get final carry from last process
            if (pid == P - 1) {
                int finalCarry;
                MPI_Recv(&finalCarry, 1, MPI_INT, pid, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (finalCarry != 0) {
                    outA << finalCarry;
                }
            }
            delete[] results[pid - 1];
        }

        // Wait for all sends to complete before freeing memory
        MPI_Waitall((P - 1) * 2, sendRequests, MPI_STATUSES_IGNORE);
        
        // Clean up
        for (int i = 0; i < P - 1; i++) {
            delete[] firstNumbers[i];
            delete[] secondNumbers[i];
        }
        delete[] firstNumbers;
        delete[] secondNumbers;
        delete[] sendRequests;
        delete[] recvRequests;
        delete[] results;
        delete[] batchSizes;
        outA.close();
    }
    else {
        MPI_Request requests[2];
        const int dimension = N_Max / (P - 1);
        const int extra = N_Max % (P - 1);
        const int batchSize = dimension + ((rank - 1) < extra);

        int *firstNumber = new int[batchSize];
        int *secondNumber = new int[batchSize];
        int* result = new int[batchSize];
        
        // Asynchronously receive digits from process 0
        MPI_Irecv(firstNumber, batchSize, MPI_INT, 0, 1, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(secondNumber, batchSize, MPI_INT, 0, 2, MPI_COMM_WORLD, &requests[1]);

        // Wait for both numbers to arrive before computing
        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);

        // Compute sum
        int carry = sum(firstNumber, secondNumber, result, batchSize);

        // Handle carry from previous process
        if (rank > 1) {
            int receivedCarry = 0;
            MPI_Request carryRecvRequest;
            MPI_Irecv(&receivedCarry, 1, MPI_INT, rank - 1, 5, MPI_COMM_WORLD, &carryRecvRequest);
            MPI_Wait(&carryRecvRequest, MPI_STATUS_IGNORE);
            
            if (receivedCarry > 0) {
                passCarry(result, batchSize, receivedCarry);
                carry += receivedCarry;
            }
        }
        
        // Send carry to next process
        if (rank < (P - 1)) {
            MPI_Request carrySendRequest;
            MPI_Isend(&carry, 1, MPI_INT, rank + 1, 5, MPI_COMM_WORLD, &carrySendRequest);
            MPI_Wait(&carrySendRequest, MPI_STATUS_IGNORE);
        }

        // Send result back to process 0
        MPI_Request resultRequest;
        MPI_Isend(result, batchSize, MPI_INT, 0, 3, MPI_COMM_WORLD, &resultRequest);
        MPI_Wait(&resultRequest, MPI_STATUS_IGNORE);
        
        // Last process sends final carry
        if (rank == P - 1) {
            MPI_Send(&carry, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
        }

        delete[] firstNumber;
        delete[] secondNumber;
        delete[] result;
    }
}

int AsyncCalculation::sum(const int *firstNumber, const int *secondNumber, int *result, const int size) {
    int carry = 0;
    for (int i = 0; i < size; i++) {
        result[i] = (firstNumber[i] + secondNumber[i] + carry) % 10;
        carry = (firstNumber[i] + secondNumber[i] + carry) / 10;
    }
    return carry;
}

void AsyncCalculation::passCarry(int *number, const int size, int &carry) {
    for (int i = 0; i < size; i++) {
        const int value = number[i] + carry;
        number[i] = value % 10;
        carry = value / 10;
    }
}

