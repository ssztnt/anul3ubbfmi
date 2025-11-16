#include "LexicalAnalyzer.h"
#include <iostream>
#include <string>

using namespace std;

void displayMenu() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   ANALIZOR LEXICAL - Lab 2 Partea 2   ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "1. Analizeaza fisier sursa" << endl;
    cout << "2. Analizeaza string (input manual)" << endl;
    cout << "3. Afiseaza Tabela de Simboluri (TS)" << endl;
    cout << "4. Afiseaza Forma Interna a Programului (FIP)" << endl;
    cout << "5. Afiseaza Erorile" << endl;
    cout << "6. Afiseaza toate rezultatele" << endl;
    cout << "7. Salveaza FIP in fisier" << endl;
    cout << "8. Salveaza TS in fisier" << endl;
    cout << "9. Test rapid (p1.txt)" << endl;
    cout << "0. Iesire" << endl;
    cout << "────────────────────────────────────────" << endl;
    cout << "Alegeti optiunea: ";
}

int main() {
    LexicalAnalyzer analyzer;
    
    // Incarca automatele finite
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║    INITIALIZARE ANALIZOR LEXICAL       ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    if (!analyzer.loadAutomata("identifier_dfa.txt", 
                                "integer_dfa.txt", 
                                "real_constant_dfa.txt")) {
        cerr << "\nEroare: Nu s-au putut incarca automatele finite!" << endl;
        cerr << "Asigurati-va ca fisierele DFA sunt in directorul curent." << endl;
        return 1;
    }
    
    cout << "\n✓ Analizor lexical initializat cu succes!" << endl;
    
    int choice;
    bool analyzed = false;
    
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Ignora newline-ul ramas
        
        switch (choice) {
            case 0:
                cout << "\nLa revedere!" << endl;
                return 0;
                
            case 1: {
                cout << "\nIntroduceti numele fisierului sursa: ";
                string filename;
                getline(cin, filename);
                
                cout << "\nAnaliza fisier: " << filename << endl;
                analyzed = analyzer.analyzeFile(filename);
                
                if (analyzed) {
                    cout << "\n✓ Analiza finalizata cu succes!" << endl;
                } else {
                    cout << "\n✗ Analiza finalizata cu erori!" << endl;
                }
                break;
            }
            
            case 2: {
                cout << "\nIntroduceti codul sursa (terminati cu o linie goala):" << endl;
                string input, line;
                
                while (getline(cin, line) && !line.empty()) {
                    input += line + "\n";
                }
                
                cout << "\nAnaliza string..." << endl;
                analyzed = analyzer.analyzeString(input);
                
                if (analyzed) {
                    cout << "\n✓ Analiza finalizata cu succes!" << endl;
                } else {
                    cout << "\n✗ Analiza finalizata cu erori!" << endl;
                }
                break;
            }
            
            case 3:
                if (!analyzed) {
                    cout << "\nVa rugam sa efectuati mai intai o analiza (optiunea 1 sau 2)." << endl;
                } else {
                    analyzer.displaySymbolTable();
                }
                break;
                
            case 4:
                if (!analyzed) {
                    cout << "\nVa rugam sa efectuati mai intai o analiza (optiunea 1 sau 2)." << endl;
                } else {
                    analyzer.displayFIP();
                }
                break;
                
            case 5:
                if (!analyzed) {
                    cout << "\nVa rugam sa efectuati mai intai o analiza (optiunea 1 sau 2)." << endl;
                } else {
                    analyzer.displayErrors();
                }
                break;
                
            case 6:
                if (!analyzed) {
                    cout << "\nVa rugam sa efectuati mai intai o analiza (optiunea 1 sau 2)." << endl;
                } else {
                    analyzer.displayAll();
                }
                break;
                
            case 7: {
                if (!analyzed) {
                    cout << "\nVa rugam sa efectuati mai intai o analiza (optiunea 1 sau 2)." << endl;
                    break;
                }
                
                cout << "\nIntroduceti numele fisierului pentru FIP: ";
                string filename;
                getline(cin, filename);
                analyzer.saveFIP(filename);
                break;
            }
            
            case 8: {
                if (!analyzed) {
                    cout << "\nVa rugam sa efectuati mai intai o analiza (optiunea 1 sau 2)." << endl;
                    break;
                }
                
                cout << "\nIntroduceti numele fisierului pentru TS: ";
                string filename;
                getline(cin, filename);
                analyzer.saveSymbolTable(filename);
                break;
            }
            
            case 9: {
                cout << "\n=== Test Rapid cu p1.txt ===" << endl;
                analyzed = analyzer.analyzeFile("p1.txt");
                
                if (analyzed) {
                    cout << "\n✓ Analiza finalizata cu succes!" << endl;
                    analyzer.displayAll();
                } else {
                    cout << "\n✗ Analiza finalizata cu erori!" << endl;
                    analyzer.displayAll();
                }
                break;
            }
            
            default:
                cout << "\nOptiune invalida! Incercati din nou." << endl;
                break;
        }
    }
    
    return 0;
}

