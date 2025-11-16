//
// Variant 1.1 - Optimized Standard Communication
// Processes start addition before receiving carry
//

#ifndef TEMA_3_OPTIMIZEDCALCULATION_H
#define TEMA_3_OPTIMIZEDCALCULATION_H


class OptimizedCalculation {
private:
    int P;
    int N_Max;
public:
    OptimizedCalculation(const int P, const int N_Max) {
        this->P = P;
        this->N_Max = N_Max;
    }
    void run();
    void calculator(int rank);
    int sum(const int *firstNumber, const int *secondNumber, int *result, int size);
    void passCarry(int* number, int size, int &carry);
};


#endif //TEMA_3_OPTIMIZEDCALCULATION_H

