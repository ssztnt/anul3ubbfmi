#include "FiniteAutomaton.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

FiniteAutomaton::FiniteAutomaton() : isDeterministic(true) {}

void FiniteAutomaton::trim(string& s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), s.end());
}

vector<string> FiniteAutomaton::split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool FiniteAutomaton::readFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return false;
    }

    string line;
    int section = 0; // 0=states, 1=alphabet, 2=initial, 3=final, 4=transitions

    while (getline(file, line)) {
        trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Check for section markers
        if (line == "STATES:") {
            section = 0;
            continue;
        } else if (line == "ALPHABET:") {
            section = 1;
            continue;
        } else if (line == "INITIAL:") {
            section = 2;
            continue;
        } else if (line == "FINAL:") {
            section = 3;
            continue;
        } else if (line == "TRANSITIONS:") {
            section = 4;
            continue;
        }

        // Process data based on current section
        switch (section) {
            case 0: { // States
                vector<string> stateList = split(line, ',');
                for (const auto& state : stateList) {
                    states.insert(state);
                }
                break;
            }
            case 1: { // Alphabet
                vector<string> symbols = split(line, ',');
                for (const auto& symbol : symbols) {
                    if (symbol.length() == 1) {
                        alphabet.insert(symbol[0]);
                    } else if (symbol == "SPACE") {
                        alphabet.insert(' ');
                    }
                }
                break;
            }
            case 2: { // Initial state
                initialState = line;
                break;
            }
            case 3: { // Final states
                vector<string> finalList = split(line, ',');
                for (const auto& state : finalList) {
                    finalStates.insert(state);
                }
                break;
            }
            case 4: { // Transitions: format "state, symbol->nextState1,nextState2,..."
                size_t arrowPos = line.find("->");
                if (arrowPos != string::npos) {
                    string left = line.substr(0, arrowPos);
                    string right = line.substr(arrowPos + 2);
                    
                    trim(left);
                    trim(right);
                    
                    size_t commaPos = left.find(',');
                    if (commaPos != string::npos) {
                        string state = left.substr(0, commaPos);
                        string symbolStr = left.substr(commaPos + 1);
                        trim(state);
                        trim(symbolStr);
                        
                        char symbol;
                        if (symbolStr == "SPACE") {
                            symbol = ' ';
                        } else if (!symbolStr.empty()) {
                            symbol = symbolStr[0];
                        } else {
                            continue;
                        }
                        
                        vector<string> nextStates = split(right, ',');
                        for (const auto& nextState : nextStates) {
                            transitions[{state, symbol}].insert(nextState);
                        }
                    }
                }
                break;
            }
        }
    }

    file.close();
    checkIfDeterministic();
    return true;
}

void FiniteAutomaton::readFromKeyboard() {
    cout << "=== Citire Automat Finit ===" << endl;
    
    // Citire stari
    cout << "Introduceti starile (separate prin virgula): ";
    string line;
    getline(cin, line);
    vector<string> stateList = split(line, ',');
    for (const auto& state : stateList) {
        states.insert(state);
    }
    
    // Citire alfabet
    cout << "Introduceti alfabetul (separate prin virgula): ";
    getline(cin, line);
    vector<string> symbols = split(line, ',');
    for (const auto& symbol : symbols) {
        if (symbol.length() == 1) {
            alphabet.insert(symbol[0]);
        }
    }
    
    // Citire stare initiala
    cout << "Introduceti starea initiala: ";
    getline(cin, initialState);
    trim(initialState);
    
    // Citire stari finale
    cout << "Introduceti starile finale (separate prin virgula): ";
    getline(cin, line);
    vector<string> finalList = split(line, ',');
    for (const auto& state : finalList) {
        finalStates.insert(state);
    }
    
    // Citire tranzitii
    cout << "Introduceti tranzitiile (format: state,symbol->nextState)" << endl;
    cout << "Introduceti 'done' pentru a termina:" << endl;
    while (true) {
        getline(cin, line);
        trim(line);
        if (line == "done") break;
        
        size_t arrowPos = line.find("->");
        if (arrowPos != string::npos) {
            string left = line.substr(0, arrowPos);
            string right = line.substr(arrowPos + 2);
            
            trim(left);
            trim(right);
            
            size_t commaPos = left.find(',');
            if (commaPos != string::npos) {
                string state = left.substr(0, commaPos);
                string symbolStr = left.substr(commaPos + 1);
                trim(state);
                trim(symbolStr);
                
                if (!symbolStr.empty()) {
                    char symbol = symbolStr[0];
                    transitions[{state, symbol}].insert(right);
                }
            }
        }
    }
    
    checkIfDeterministic();
}

void FiniteAutomaton::displayStates() const {
    cout << "\n=== Multimea Starilor (Q) ===" << endl;
    cout << "{ ";
    for (auto it = states.begin(); it != states.end(); ++it) {
        if (it != states.begin()) cout << ", ";
        cout << *it;
    }
    cout << " }" << endl;
}

void FiniteAutomaton::displayAlphabet() const {
    cout << "\n=== Alfabetul (Σ) ===" << endl;
    cout << "{ ";
    for (auto it = alphabet.begin(); it != alphabet.end(); ++it) {
        if (it != alphabet.begin()) cout << ", ";
        if (*it == ' ') {
            cout << "SPACE";
        } else {
            cout << "'" << *it << "'";
        }
    }
    cout << " }" << endl;
}

void FiniteAutomaton::displayTransitions() const {
    cout << "\n=== Tranzitiile (δ) ===" << endl;
    for (const auto& trans : transitions) {
        cout << "δ(" << trans.first.first << ", ";
        if (trans.first.second == ' ') {
            cout << "SPACE";
        } else {
            cout << "'" << trans.first.second << "'";
        }
        cout << ") = { ";
        for (auto it = trans.second.begin(); it != trans.second.end(); ++it) {
            if (it != trans.second.begin()) cout << ", ";
            cout << *it;
        }
        cout << " }" << endl;
    }
}

void FiniteAutomaton::displayFinalStates() const {
    cout << "\n=== Multimea Starilor Finale (F) ===" << endl;
    cout << "{ ";
    for (auto it = finalStates.begin(); it != finalStates.end(); ++it) {
        if (it != finalStates.begin()) cout << ", ";
        cout << *it;
    }
    cout << " }" << endl;
}

void FiniteAutomaton::displayAll() const {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║      AUTOMAT FINIT - Toate Datele      ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    displayStates();
    displayAlphabet();
    
    cout << "\n=== Starea Initiala (q0) ===" << endl;
    cout << initialState << endl;
    
    displayFinalStates();
    displayTransitions();
    
    cout << "\n=== Tip ===" << endl;
    cout << (isDeterministic ? "DFA (Determinist)" : "NFA (Nedeterminist)") << endl;
}

bool FiniteAutomaton::checkIfDeterministic() {
    isDeterministic = true;
    
    // Un automat este determinist daca pentru fiecare (state, symbol)
    // exista cel mult o tranzitie
    for (const auto& trans : transitions) {
        if (trans.second.size() > 1) {
            isDeterministic = false;
            break;
        }
    }
    
    return isDeterministic;
}

bool FiniteAutomaton::isSequenceAccepted(const string& sequence) const {
    if (!isDeterministic) {
        cout << "Atentie: Automatul nu este determinist. Rezultatul poate fi incorect." << endl;
    }
    
    string currentState = initialState;
    
    for (char symbol : sequence) {
        auto it = transitions.find({currentState, symbol});
        if (it == transitions.end() || it->second.empty()) {
            return false; // Nu exista tranzitie
        }
        currentState = *(it->second.begin()); // Luam prima (si singura pentru DFA) stare
    }
    
    return finalStates.find(currentState) != finalStates.end();
}

string FiniteAutomaton::longestAcceptedPrefix(const string& sequence) const {
    if (!isDeterministic) {
        cout << "Atentie: Automatul nu este determinist. Rezultatul poate fi incorect." << endl;
    }
    
    string currentState = initialState;
    string longestPrefix = "";
    
    // Daca starea initiala este finala, prefixul vid este acceptat
    if (finalStates.find(currentState) != finalStates.end()) {
        longestPrefix = "";
    }
    
    for (size_t i = 0; i < sequence.length(); i++) {
        char symbol = sequence[i];
        auto it = transitions.find({currentState, symbol});
        
        if (it == transitions.end() || it->second.empty()) {
            break; // Nu mai exista tranzitie
        }
        
        currentState = *(it->second.begin());
        
        // Verificam daca am ajuns intr-o stare finala
        if (finalStates.find(currentState) != finalStates.end()) {
            longestPrefix = sequence.substr(0, i + 1);
        }
    }
    
    return longestPrefix;
}