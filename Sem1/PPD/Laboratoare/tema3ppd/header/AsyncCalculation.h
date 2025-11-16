//
// Created by vladb on 11/11/2025.
//

#ifndef TEMA_3_ASYNCCALCULATION_H
#define TEMA_3_ASYNCCALCULATION_H


class AsyncCalculation {
private:
    int P;
    int N_Max;
public:
    AsyncCalculation(const int P, const int N_Max) {
        this->P = P;
        this->N_Max = N_Max;
    }
    void run();
    void calculator(int rank);
    int sum(const int *firstNumber, const int *secondNumber, int *result, int size);
    void passCarry(int* number,int size,int &carry);
};


#endif //TEMA_3_ASYNCCALCULATION_H