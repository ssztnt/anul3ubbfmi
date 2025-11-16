#ifndef FINITE_AUTOMATON_H
#define FINITE_AUTOMATON_H

#include <string>
#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class FiniteAutomaton {
private:
    std::set<std::string> states;           // Q - multimea starilor
    std::set<char> alphabet;                // Σ - alfabetul
    std::map<std::pair<std::string, char>, std::set<std::string>> transitions; // δ - functia de tranzitie
    std::string initialState;               // q0 - starea initiala
    std::set<std::string> finalStates;      // F - multimea starilor finale
    bool isDeterministic;                   // flag pentru DFA vs NFA

public:
    // Constructor
    FiniteAutomaton();

    // Citire din fisier
    bool readFromFile(const std::string& filename);

    // Citire de la tastatura
    void readFromKeyboard();

    // Afisare elemente
    void displayStates() const;
    void displayAlphabet() const;
    void displayTransitions() const;
    void displayFinalStates() const;
    void displayAll() const;

    // Verificare daca automatul este determinist
    bool checkIfDeterministic();

    // Verificare secventa (pentru DFA)
    bool isSequenceAccepted(const std::string& sequence) const;

    // Cel mai lung prefix acceptat (pentru DFA)
    std::string longestAcceptedPrefix(const std::string& sequence) const;

    // Getters
    bool getIsDeterministic() const { return isDeterministic; }
    const std::set<std::string>& getStates() const { return states; }
    const std::set<char>& getAlphabet() const { return alphabet; }
    const std::string& getInitialState() const { return initialState; }
    const std::set<std::string>& getFinalStates() const { return finalStates; }

private:
    // Functii helper
    void trim(std::string& s);
    std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif // FINITE_AUTOMATON_H

