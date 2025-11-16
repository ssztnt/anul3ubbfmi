#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "FiniteAutomaton.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

// Structura pentru un token
struct Token {
    std::string type;      // Tipul tokenului (ID, CONST_INT, CONST_REAL, etc.)
    std::string value;     // Valoarea tokenului
    int line;              // Linia pe care apare
    int column;            // Coloana pe care apare
};

// Structura pentru un simbol in tabela de simboluri
struct Symbol {
    std::string value;     // Valoarea simbolului
    int index;             // Index-ul in TS
};

class LexicalAnalyzer {
private:
    // Automate finite pentru diferite categorii de tokeni
    FiniteAutomaton identifierFA;
    FiniteAutomaton integerFA;
    FiniteAutomaton realFA;
    
    // Cuvinte cheie din limbaj
    std::set<std::string> keywords;
    
    // Operatori si delimitatori
    std::set<std::string> operators;
    std::set<std::string> delimiters;
    
    // Tabela de simboluri (TS) - pentru identificatori si constante
    std::map<std::string, int> symbolTable;
    int symbolTableIndex;
    
    // Forma Interna a Programului (FIP)
    // (cod_atom, pozitie_TS) - pentru ID si CONST folosim pozitia din TS
    // pentru cuvinte cheie, operatori, delimitatori folosim 0
    std::vector<std::pair<std::string, int>> fip;
    
    // Lista de erori
    std::vector<std::string> errors;
    
    // Functii helper
    bool isWhitespace(char c);
    bool isOperatorChar(char c);
    bool isDelimiterChar(char c);
    std::string getNextToken(const std::string& input, size_t& pos, int& line, int& column);
    bool isKeyword(const std::string& token);
    bool isOperator(const std::string& token);
    bool isDelimiter(const std::string& token);
    int addToSymbolTable(const std::string& value);
    void addToFIP(const std::string& code, int position);
    void skipWhitespace(const std::string& input, size_t& pos, int& line, int& column);
    
public:
    LexicalAnalyzer();
    
    // Incarca automatele finite
    bool loadAutomata(const std::string& identifierFile,
                      const std::string& integerFile,
                      const std::string& realFile);
    
    // Analizeaza un fisier sursa
    bool analyzeFile(const std::string& filename);
    
    // Analizeaza un string
    bool analyzeString(const std::string& input);
    
    // Afisare rezultate
    void displaySymbolTable() const;
    void displayFIP() const;
    void displayErrors() const;
    void displayAll() const;
    
    // Salvare rezultate in fisiere
    void saveFIP(const std::string& filename) const;
    void saveSymbolTable(const std::string& filename) const;
    
    // Getters
    bool hasErrors() const { return !errors.empty(); }
    const std::vector<std::string>& getErrors() const { return errors; }
};

#endif // LEXICAL_ANALYZER_H

