#pragma once
class SequentialCalculation
{
private:
	int* numberOne;
	int* numberTwo;
	int N1;
	int N2;
public:
	SequentialCalculation(int *number_one, int *number_two, int n1, int n2)
		: numberOne(number_one),
		  numberTwo(number_two),
		  N1(n1),
		  N2(n2) {
	}

	int* calculate();

};

