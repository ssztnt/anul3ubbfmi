#include "../header/GenerateNumber.h"
#include <fstream>
using namespace std;


void GenerateNumber::generateNumber(const string &fileName, const int numberOfDigits) {
    int *table = new int[numberOfDigits];
    for (int i = 0; i < numberOfDigits - 1; i++) {
        table[i] = rand() % 10;
    }
    table[numberOfDigits - 1] = (rand() % 9) + 1;

    ofstream out(fileName);
    out << numberOfDigits << endl;
    for (int i = 0; i < numberOfDigits; i++) {
        out << table[i] << " ";
    }
    delete[] table;
}

int *GenerateNumber::readNumber(const string &fileName) {
    ifstream in(fileName);
    if (!in) {
        cerr << "Number file could not be opened" << endl;
    }
    int numberOfDigits;
    in >> numberOfDigits;
    int *table = new int[numberOfDigits];
    int digit;
    for (int i = 0; i < numberOfDigits; i++) {
        in >> digit;
        table[i] = digit;
    }
    return table;
}
int *GenerateNumber::readNumberP(const string &fileName,const int P) {
    ifstream in(fileName);
    if (!in) {
        cerr << "Number file could not be opened" << endl;
    }
    int numberOfDigits;
    int* table;
    in >> numberOfDigits;
    if (numberOfDigits < P) {
        table=new int[P];
        for (int i = numberOfDigits; i < P; i++) {
            table[i] = 0;
        }
    }
    else {
       table = new int[numberOfDigits];
    }
    int digit;
    for (int i = 0; i < numberOfDigits; i++) {
        in >> digit;
        table[i] = digit;
    }
    return table;
}

void GenerateNumber::writeNumber(const string &fileName, int *number, const int numberOfDigits) {
    ofstream out(fileName);
    if (!out) {
        cerr << "Number file could not be opened" << endl;
    }
    cout << numberOfDigits << endl;
    for (int i = 0; i < numberOfDigits; i++) {
        out << number[i] << " ";
    }
}

int *GenerateNumber::readNumberBlock(const string &fileName, const int offset, const int size) {
    ifstream in(fileName);
    int numberOfDigits;
    in >> numberOfDigits;
    int *table = new int[size];
    int initialIndex = 0;
    int idx;
    for (int i = 0; i < numberOfDigits && (numberOfDigits > offset); i++) {
        in >> idx;
        if (i >= offset && initialIndex < size) {
            table[initialIndex] = idx;
            initialIndex++;
        }
    }
    while (initialIndex < size) {
        table[initialIndex] = 0;
        initialIndex++;
    }
    return table;
}
