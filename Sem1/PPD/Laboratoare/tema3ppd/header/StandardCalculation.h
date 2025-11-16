
#ifndef TEMA_3_STANDARDCALCULATION_H
#define TEMA_3_STANDARDCALCULATION_H


class StandardCalculation {
private:
    int P;
    int N_Max;
public:
    StandardCalculation(const int P, const int N_Max) {
        this->P = P;
        this->N_Max = N_Max;
    }
    void run();
    void calculator(int rank);
    int sum(const int *firstNumber, const int *secondNumber, int *result, int size);
    void passCarry(int* number,int size,int &carry);
};


#endif