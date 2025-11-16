#include "FiniteAutomaton.h"
#include <iostream>
#include <limits>

void displayMenu() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║       MENIU AUTOMAT FINIT              ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    std::cout << "1. Citeste automat din fisier" << std::endl;
    std::cout << "2. Citeste automat de la tastatura" << std::endl;
    std::cout << "3. Afiseaza multimea starilor" << std::endl;
    std::cout << "4. Afiseaza alfabetul" << std::endl;
    std::cout << "5. Afiseaza tranzitiile" << std::endl;
    std::cout << "6. Afiseaza starile finale" << std::endl;
    std::cout << "7. Afiseaza toate elementele" << std::endl;
    std::cout << "8. Verifica daca o secventa este acceptata (DFA)" << std::endl;
    std::cout << "9. Gaseste cel mai lung prefix acceptat (DFA)" << std::endl;
    std::cout << "0. Iesire" << std::endl;
    std::cout << "\nAlegeti optiunea: ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    FiniteAutomaton fa;
    bool loaded = false;
    int choice;

    

    while (true) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cout << "Eroare: Introduceti un numar valid!" << std::endl;
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                std::string filename;
                std::cout << "Introduceti numele fisierului: ";
                std::getline(std::cin, filename);
                if (fa.readFromFile(filename)) {
                    std::cout << "Automatul a fost incarcat cu succes!" << std::endl;
                    loaded = true;
                } else {
                    std::cout << "Eroare la incarcarea automatului!" << std::endl;
                }
                break;
            }
            case 2: {
                fa.readFromKeyboard();
                std::cout << "Automatul a fost introdus cu succes!" << std::endl;
                loaded = true;
                break;
            }
            case 3: {
                if (!loaded) {
                    std::cout << "Eroare: Mai intai trebuie sa incarcati un automat!" << std::endl;
                } else {
                    fa.displayStates();
                }
                break;
            }
            case 4: {
                if (!loaded) {
                    std::cout << "Eroare: Mai intai trebuie sa incarcati un automat!" << std::endl;
                } else {
                    fa.displayAlphabet();
                }
                break;
            }
            case 5: {
                if (!loaded) {
                    std::cout << "Eroare: Mai intai trebuie sa incarcati un automat!" << std::endl;
                } else {
                    fa.displayTransitions();
                }
                break;
            }
            case 6: {
                if (!loaded) {
                    std::cout << "Eroare: Mai intai trebuie sa incarcati un automat!" << std::endl;
                } else {
                    fa.displayFinalStates();
                }
                break;
            }
            case 7: {
                if (!loaded) {
                    std::cout << "Eroare: Mai intai trebuie sa incarcati un automat!" << std::endl;
                } else {
                    fa.displayAll();
                }
                break;
            }
            case 8: {
                if (!loaded) {
                    std::cout << "Eroare: Mai intai trebuie sa incarcati un automat!" << std::endl;
                } else if (!fa.getIsDeterministic()) {
                    std::cout << "Atentie: Automatul nu este determinist!" << std::endl;
                    std::cout << "Continuati oricum? (d/n): ";
                    char confirm;
                    std::cin >> confirm;
                    clearInputBuffer();
                    if (confirm != 'd' && confirm != 'D') {
                        break;
                    }
                }
                
                if (loaded) {
                    std::string sequence;
                    std::cout << "Introduceti secventa de verificat: ";
                    std::getline(std::cin, sequence);
                    
                    bool accepted = fa.isSequenceAccepted(sequence);
                    std::cout << "\nSecventa \"" << sequence << "\" este ";
                    std::cout << (accepted ? "ACCEPTATA" : "RESPINSA") << " de automat." << std::endl;
                }
                break;
            }
            case 9: {
                if (!loaded) {
                    std::cout << "Eroare: Mai intai trebuie sa incarcati un automat!" << std::endl;
                } else if (!fa.getIsDeterministic()) {
                    std::cout << "Atentie: Automatul nu este determinist!" << std::endl;
                    std::cout << "Continuati oricum? (d/n): ";
                    char confirm;
                    std::cin >> confirm;
                    clearInputBuffer();
                    if (confirm != 'd' && confirm != 'D') {
                        break;
                    }
                }
                
                if (loaded) {
                    std::string sequence;
                    std::cout << "Introduceti secventa: ";
                    std::getline(std::cin, sequence);
                    
                    std::string prefix = fa.longestAcceptedPrefix(sequence);
                    if (prefix.empty()) {
                        std::cout << "\nNu exista prefix acceptat (sau prefixul vid este cel mai lung)." << std::endl;
                    } else {
                        std::cout << "\nCel mai lung prefix acceptat: \"" << prefix << "\"" << std::endl;
                        std::cout << "Lungime: " << prefix.length() << " caractere" << std::endl;
                    }
                }
                break;
            }
            case 0: {
                std::cout << "\nLa revedere!" << std::endl;
                return 0;
            }
            default: {
                std::cout << "Optiune invalida! Incercati din nou." << std::endl;
                break;
            }
        }
    }

    return 0;
}

