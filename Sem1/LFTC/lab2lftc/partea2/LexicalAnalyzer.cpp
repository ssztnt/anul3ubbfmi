#include "LexicalAnalyzer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iomanip>

LexicalAnalyzer::LexicalAnalyzer() : symbolTableIndex(0) {
    // Initializare cuvinte cheie (subset MLP - mini-limbaj de programare)
    keywords = {
        "int", "float", "char", "string", "bool",
        "if", "else", "while", "for", "do",
        "return", "void", "main",
        "read", "write", "print",  // ← ADĂUGAT 'print'
        "true", "false"
    };
    
    // Initializare operatori
    operators = {
        "+", "-", "*", "/", "%",
        "=", "==", "!=", "<", ">", "<=", ">=",
        "&&", "||", "!",
        "++", "--",
        "+=", "-=", "*=", "/="
    };
    
    // Initializare delimitatori
    delimiters = {
        "(", ")", "{", "}", "[", "]",
        ";", ",", ".", ":"
    };
}

bool LexicalAnalyzer::loadAutomata(const std::string& identifierFile,
                                    const std::string& integerFile,
                                    const std::string& realFile) {
    std::cout << "Incarcare automate finite..." << std::endl;
    
    if (!identifierFA.readFromFile(identifierFile)) {
        std::cerr << "Eroare: Nu s-a putut incarca DFA-ul pentru identificatori: " 
                  << identifierFile << std::endl;
        return false;
    }
    std::cout << "✓ DFA pentru identificatori incarcat" << std::endl;
    
    if (!integerFA.readFromFile(integerFile)) {
        std::cerr << "Eroare: Nu s-a putut incarca DFA-ul pentru constante intregi: " 
                  << integerFile << std::endl;
        return false;
    }
    std::cout << "✓ DFA pentru constante intregi incarcat" << std::endl;
    
    if (!realFA.readFromFile(realFile)) {
        std::cerr << "Eroare: Nu s-a putut incarca DFA-ul pentru constante reale: " 
                  << realFile << std::endl;
        return false;
    }
    std::cout << "✓ DFA pentru constante reale incarcat" << std::endl;
    
    return true;
}

bool LexicalAnalyzer::isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool LexicalAnalyzer::isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '=' || c == '!' || c == '<' || c == '>' ||
           c == '&' || c == '|';
}

bool LexicalAnalyzer::isDelimiterChar(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' ||
           c == '[' || c == ']' || c == ';' || c == ',' ||
           c == '.' || c == ':';
}

void LexicalAnalyzer::skipWhitespace(const std::string& input, size_t& pos, 
                                      int& line, int& column) {
    while (pos < input.length()) {
        // Skip whitespace
        if (isWhitespace(input[pos])) {
            if (input[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
        // Skip single-line comments //
        else if (pos + 1 < input.length() && input[pos] == '/' && input[pos + 1] == '/') {
            // Skip until end of line
            while (pos < input.length() && input[pos] != '\n') {
                pos++;
                column++;
            }
            // Skip the newline too
            if (pos < input.length() && input[pos] == '\n') {
                pos++;
                line++;
                column = 1;
            }
        }
        // Skip multi-line comments /* */
        else if (pos + 1 < input.length() && input[pos] == '/' && input[pos + 1] == '*') {
            pos += 2;
            column += 2;
            // Skip until we find */
            while (pos + 1 < input.length()) {
                if (input[pos] == '*' && input[pos + 1] == '/') {
                    pos += 2;
                    column += 2;
                    break;
                }
                if (input[pos] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                pos++;
            }
        }
        else {
            break; // Not whitespace or comment
        }
    }
}

std::string LexicalAnalyzer::getNextToken(const std::string& input, size_t& pos, 
                                          int& line, int& column) {
    skipWhitespace(input, pos, line, column);
    
    if (pos >= input.length()) {
        return "";
    }
    
    std::string token;
    
    // Verifica daca e operator
    if (isOperatorChar(input[pos])) {
        token += input[pos++];
        column++;
        
        // Verifica operatori cu 2 caractere
        if (pos < input.length()) {
            std::string twoChar = token + input[pos];
            if (isOperator(twoChar)) {
                token = twoChar;
                pos++;
                column++;
            }
        }
        return token;
    }
    
    // Verifica daca e delimitator
    if (isDelimiterChar(input[pos])) {
        token += input[pos++];
        column++;
        return token;
    }
    
    // Construieste token pana la whitespace, operator sau delimitator
    while (pos < input.length() && 
           !isWhitespace(input[pos]) && 
           !isOperatorChar(input[pos]) && 
           !isDelimiterChar(input[pos])) {
        token += input[pos++];
        column++;
    }
    
    return token;
}

bool LexicalAnalyzer::isKeyword(const std::string& token) {
    return keywords.find(token) != keywords.end();
}

bool LexicalAnalyzer::isOperator(const std::string& token) {
    return operators.find(token) != operators.end();
}

bool LexicalAnalyzer::isDelimiter(const std::string& token) {
    return delimiters.find(token) != delimiters.end();
}

int LexicalAnalyzer::addToSymbolTable(const std::string& value) {
    // Verifica daca valoarea exista deja in TS
    auto it = symbolTable.find(value);
    if (it != symbolTable.end()) {
        return it->second;
    }
    
    // Adauga valoarea noua in TS
    symbolTable[value] = symbolTableIndex;
    return symbolTableIndex++;
}

void LexicalAnalyzer::addToFIP(const std::string& code, int position) {
    fip.push_back({code, position});
}

bool LexicalAnalyzer::analyzeFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Eroare: Nu se poate deschide fisierul " << filename << std::endl;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();
    file.close();
    
    return analyzeString(input);
}

bool LexicalAnalyzer::analyzeString(const std::string& input) {
    // Reseteaza structurile
    symbolTable.clear();
    fip.clear();
    errors.clear();
    symbolTableIndex = 0;
    
    size_t pos = 0;
    int line = 1;
    int column = 1;
    
    std::cout << "\n=== Incepere Analiza Lexicala ===" << std::endl;
    
    while (pos < input.length()) {
        int tokenLine = line;
        int tokenColumn = column;
        
        std::string token = getNextToken(input, pos, line, column);
        
        if (token.empty()) {
            continue;
        }
        
        // std::cout << "Token: '" << token << "' la linia " << tokenLine 
        //           << ", coloana " << tokenColumn << std::endl;
        
        // Verifica daca e cuvant cheie
        if (isKeyword(token)) {
            addToFIP(token, 0);
            continue;
        }
        
        // Verifica daca e operator
        if (isOperator(token)) {
            addToFIP(token, 0);
            continue;
        }
        
        // Verifica daca e delimitator
        if (isDelimiter(token)) {
            addToFIP(token, 0);
            continue;
        }
        
        // Verifica cu automatele finite
        bool recognized = false;
        
        // 1. Incearca sa recunoasca ca numar real (mai specific)
        if (realFA.isSequenceAccepted(token)) {
            int pos = addToSymbolTable(token);
            addToFIP("CONST_REAL", pos);
            recognized = true;
        }
        // 2. Incearca sa recunoasca ca numar intreg
        else if (integerFA.isSequenceAccepted(token)) {
            int pos = addToSymbolTable(token);
            addToFIP("CONST_INT", pos);
            recognized = true;
        }
        // 3. Incearca sa recunoasca ca identificator
        else if (identifierFA.isSequenceAccepted(token)) {
            int pos = addToSymbolTable(token);
            addToFIP("ID", pos);
            recognized = true;
        }
        
        // Daca nu e recunoscut, adauga eroare
        if (!recognized) {
            std::stringstream errorMsg;
            errorMsg << "Eroare lexicala la linia " << tokenLine 
                     << ", coloana " << tokenColumn 
                     << ": Token nerecunoscut '" << token << "'";
            errors.push_back(errorMsg.str());
            std::cerr << errorMsg.str() << std::endl;
        }
    }
    
    std::cout << "=== Analiza Lexicala Finalizata ===" << std::endl;
    std::cout << "Tokeni identificati: " << fip.size() << std::endl;
    std::cout << "Simboluri in TS: " << symbolTable.size() << std::endl;
    std::cout << "Erori: " << errors.size() << std::endl;
    
    return errors.empty();
}

void LexicalAnalyzer::displaySymbolTable() const {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║        TABELA DE SIMBOLURI (TS)        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    if (symbolTable.empty()) {
        std::cout << "Tabela de simboluri este goala." << std::endl;
        return;
    }
    
    // Creeaza un vector pentru sortare dupa index
    std::vector<std::pair<std::string, int>> sortedSymbols;
    for (const auto& pair : symbolTable) {
        sortedSymbols.push_back({pair.first, pair.second});
    }
    
    std::sort(sortedSymbols.begin(), sortedSymbols.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });
    
    std::cout << std::left;
    std::cout << "┌────────┬────────────────────────────────┐" << std::endl;
    std::cout << "│ Index  │ Simbol                         │" << std::endl;
    std::cout << "├────────┼────────────────────────────────┤" << std::endl;
    
    for (const auto& pair : sortedSymbols) {
        std::cout << "│ " << std::setw(6) << pair.second 
                  << " │ " << std::setw(30) << pair.first << " │" << std::endl;
    }
    
    std::cout << "└────────┴────────────────────────────────┘" << std::endl;
}

void LexicalAnalyzer::displayFIP() const {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   FORMA INTERNA A PROGRAMULUI (FIP)    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    if (fip.empty()) {
        std::cout << "FIP este gol." << std::endl;
        return;
    }
    
    std::cout << std::left;
    std::cout << "┌────────┬────────────────────┬────────┐" << std::endl;
    std::cout << "│ Nr.    │ Cod Atom           │ Poz TS │" << std::endl;
    std::cout << "├────────┼────────────────────┼────────┤" << std::endl;
    
    for (size_t i = 0; i < fip.size(); i++) {
        std::cout << "│ " << std::setw(6) << i 
                  << " │ " << std::setw(18) << fip[i].first
                  << " │ " << std::setw(6) << fip[i].second << " │" << std::endl;
    }
    
    std::cout << "└────────┴────────────────────┴────────┘" << std::endl;
}

void LexicalAnalyzer::displayErrors() const {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║           LISTA DE ERORI               ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    if (errors.empty()) {
        std::cout << "✓ Nu exista erori lexicale!" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < errors.size(); i++) {
        std::cout << (i + 1) << ". " << errors[i] << std::endl;
    }
}

void LexicalAnalyzer::displayAll() const {
    displaySymbolTable();
    displayFIP();
    displayErrors();
}

void LexicalAnalyzer::saveFIP(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Eroare: Nu se poate deschide fisierul " << filename << std::endl;
        return;
    }
    
    file << "FORMA INTERNA A PROGRAMULUI (FIP)\n";
    file << "==================================\n\n";
    file << "Nr.\tCod Atom\t\tPoz TS\n";
    file << "---\t--------\t\t------\n";
    
    for (size_t i = 0; i < fip.size(); i++) {
        file << i << "\t" << fip[i].first << "\t\t" << fip[i].second << "\n";
    }
    
    file.close();
    std::cout << "FIP salvat in " << filename << std::endl;
}

void LexicalAnalyzer::saveSymbolTable(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Eroare: Nu se poate deschide fisierul " << filename << std::endl;
        return;
    }
    
    file << "TABELA DE SIMBOLURI (TS)\n";
    file << "========================\n\n";
    file << "Index\tSimbol\n";
    file << "-----\t------\n";
    
    // Sorteaza dupa index
    std::vector<std::pair<std::string, int>> sortedSymbols;
    for (const auto& pair : symbolTable) {
        sortedSymbols.push_back({pair.first, pair.second});
    }
    
    std::sort(sortedSymbols.begin(), sortedSymbols.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });
    
    for (const auto& pair : sortedSymbols) {
        file << pair.second << "\t" << pair.first << "\n";
    }
    
    file.close();
    std::cout << "TS salvata in " << filename << std::endl;
}

