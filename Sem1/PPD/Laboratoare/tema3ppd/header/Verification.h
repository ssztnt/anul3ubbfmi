#pragma once
#include <string>
using namespace std;

class Verification {
public:
    static bool compareResults(const string& file1, const string& file2);
    static void printComparison(const string& variantName, const string& referenceFile, const string& testFile);
    static void runAllVerifications();
};

