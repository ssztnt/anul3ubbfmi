#pragma once
#include <iostream>
#include <string>
using namespace std;
class GenerateNumber {
public :
	static void generateNumber(const string &fileName, int numberOfDigits);

	static int* readNumber(const string& fileName);

	static void writeNumber(const string &fileName, int* number,int numberOfDigits);

	static int* readNumberBlock(const string& fileName,int offset,int size);
	static int* readNumberP(const string &fileName,int N_Max);
};