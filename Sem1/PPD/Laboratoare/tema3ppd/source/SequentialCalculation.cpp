#include "../header/SequentialCalculation.h"
int* SequentialCalculation::calculate() {
	int dim,minDim;
	if (N1 > N2) {
		dim = N1 +1;
		minDim = N2;
	}
	else {
		dim = N2 +1;
		minDim = N1;
	}

	int* result = new int[dim];
	int carry = 0; 
	for(int i = 0; i <minDim; i++) {
		result[i] =( numberOne[i] + numberTwo[i] + carry) % 10;
		carry = (numberOne[i] + numberTwo[i] + carry) / 10;
	}

	if (N1 > minDim) {
		for (int i = minDim; i < N1; i++) {
			result[i] = (numberOne[i] + carry) % 10;
			carry = (numberOne[i] + carry) / 10;
		}
		if (carry) {
			result[dim - 1] = carry;
		}
	}
	else if (N2 > minDim) {
		for (int i = minDim; i < N2; i++) {
			result[i] = (numberTwo[i] + carry) % 10;
			carry = (numberTwo[i] + carry) / 10;
		}
		if (carry) {
			result[dim - 1] = carry;
		}
	}
	else if (carry) {
			result[dim - 1] = carry;
		}
	return result;
	}