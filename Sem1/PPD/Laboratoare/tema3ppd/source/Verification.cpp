#include "../header/Verification.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

string normalizeContent(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    
    // Remove all whitespace
    content.erase(remove_if(content.begin(), content.end(), ::isspace), content.end());
    
    return content;
}

bool Verification::compareResults(const string& file1, const string& file2) {
    string content1 = normalizeContent(file1);
    string content2 = normalizeContent(file2);
    
    if (content1.empty() || content2.empty()) {
        cerr << "Error: Could not read files " << file1 << " or " << file2 << endl;
        return false;
    }
    
    return content1 == content2;
}

void Verification::printComparison(const string& variantName, const string& referenceFile, const string& testFile) {
    bool match = compareResults(referenceFile, testFile);
    
    if (match) {
        cout << "[OK] " << variantName << " matches sequential result" << endl;
    } else {
        cout << "[FAIL] " << variantName << " does NOT match sequential result!" << endl;
    }
}

void Verification::runAllVerifications() {
    cout << "\n========== RESULT VERIFICATION ==========" << endl;
    printComparison("Variant 1 (Standard)", "result.txt", "result1.txt");
    printComparison("Variant 2 (Scatter)", "result.txt", "resultScatter.txt");
    printComparison("Variant 3 (Async)", "result.txt", "resultAsync.txt");
    printComparison("Variant 1.1 (Optimized)", "result.txt", "resultOptimized.txt");
    cout << "=========================================" << endl;
}

