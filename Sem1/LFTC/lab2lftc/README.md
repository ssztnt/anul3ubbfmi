# Laborator 2 - Automate Finite (FA)
## LFTC - Limbaje Formale și Compilatoare

### Autor
Student: [Numele tău]  
Grupa: [Grupa ta]  
Anul Universitar: 2024-2025

---

## Descrierea Temei

Implementare în C++ a unui program pentru lucrul cu automate finite (FA). Programul permite:

1. **Citirea** unui automat finit din fișier sau de la tastatură
2. **Afișarea** elementelor automatului (stări, alfabet, tranziții, stări finale)
3. **Verificarea** dacă o secvență este acceptată de automat (pentru DFA)
4. **Găsirea** celui mai lung prefix acceptat dintr-o secvență (pentru DFA)

---

## Structura Proiectului

```
lab2lftc/
├── FiniteAutomaton.h       # Header-ul clasei FiniteAutomaton
├── FiniteAutomaton.cpp     # Implementarea clasei
├── main.cpp                # Programul principal cu meniu
├── Makefile                # Fișier pentru compilare automată
├── FA_FORMAT.bnf           # Gramatica BNF/EBNF pentru formatul fișierului
├── integer_dfa.txt         # DFA pentru constante întregi C/C++
├── simple_dfa.txt          # DFA simplu (exemplu de test)
└── README.md               # Acest fișier
```

---

## Compilare și Rulare

### Compilare

```bash
make
```

sau manual:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o fa_program main.cpp FiniteAutomaton.cpp
```

### Rulare

```bash
make run
```

sau:

```bash
./fa_program
```

### Curățare Fișiere

```bash
make clean      # Șterge fișierele compilate
make distclean  # Șterge tot (inclusiv backup-uri)
make rebuild    # Recompilare completă
```

---

## Formatul Fișierului de Input

Fișierul trebuie să conțină 5 secțiuni obligatorii:

```
STATES:
q0, q1, q2, ...

ALPHABET:
a, b, c, ...

INITIAL:
q0

FINAL:
q2, q3, ...

TRANSITIONS:
state,symbol -> nextState1, nextState2, ...
```

### Exemple:

#### 1. DFA Simplu (se termină în "ab")
Vezi `simple_dfa.txt`

#### 2. DFA pentru Constante Întregi C/C++
Vezi `integer_dfa.txt` - acceptă:
- Decimal: `123`, `+456`, `-789`
- Octal: `0123`, `0777`
- Hexadecimal: `0x1A`, `0xFF`, `0X2b`

---

## Funcționalități Implementate

### 1. Citire Automat Finit
- **Din fișier**: Format specific (vezi `FA_FORMAT.bnf`)
- **De la tastatură**: Input interactiv

### 2. Afișare Elemente
- Mulțimea stărilor (Q)
- Alfabetul (Σ)
- Tranzițiile (δ)
- Stările finale (F)
- Starea inițială (q₀)
- Tip: DFA vs NFA

### 3. Verificare Secvență (DFA)
Verifică dacă o secvență dată este acceptată de automat.

**Exemplu:**
- Automat: `integer_dfa.txt`
- Input: `123` → ✓ **ACCEPTAT**
- Input: `abc` → ✗ **RESPINS**

### 4. Cel Mai Lung Prefix Acceptat (DFA)
Găsește cel mai lung prefix dintr-o secvență care este acceptat de automat.

**Exemplu:**
- Automat: `integer_dfa.txt`
- Input: `123abc`
- Output: `123` (lungime: 3 caractere)

---

## Gramatica BNF/EBNF

Vezi fișierul `FA_FORMAT.bnf` pentru descrierea completă a formatului fișierului.

**Rezumat:**
```bnf
<fa_file> ::= <states_section> <alphabet_section> <initial_section> 
              <final_section> <transitions_section>

<transition> ::= <state> "," <symbol> "->" <state_list>
```

---

## Exemple de Utilizare

### Exemplu 1: Test DFA Simplu

```bash
$ ./fa_program
# Alegeți opțiunea 1
# Introduceți: simple_dfa.txt
# Alegeți opțiunea 7 (Afișare toate elementele)
# Alegeți opțiunea 8 (Verificare secvență)
# Testați: "ab", "aab", "bab", "ba"
```

### Exemplu 2: Test Constante Întregi

```bash
$ ./fa_program
# Alegeți opțiunea 1
# Introduceți: integer_dfa.txt
# Alegeți opțiunea 8 (Verificare secvență)
# Testați: "123", "-456", "0x1A", "0777"
```

### Exemplu 3: Cel Mai Lung Prefix

```bash
$ ./fa_program
# Încărcați: integer_dfa.txt
# Alegeți opțiunea 9
# Input: "123abc"
# Output: Prefixul "123"
```

---

## Clasă FiniteAutomaton

### Membri Privați:
```cpp
std::set<std::string> states;           // Q
std::set<char> alphabet;                // Σ
std::map<...> transitions;              // δ
std::string initialState;               // q₀
std::set<std::string> finalStates;      // F
bool isDeterministic;                   // DFA vs NFA
```

### Metode Publice:
- `readFromFile(filename)` - Citire din fișier
- `readFromKeyboard()` - Citire interactivă
- `displayStates()` - Afișare stări
- `displayAlphabet()` - Afișare alfabet
- `displayTransitions()` - Afișare tranziții
- `displayFinalStates()` - Afișare stări finale
- `displayAll()` - Afișare toate elementele
- `isSequenceAccepted(sequence)` - Verificare acceptare
- `longestAcceptedPrefix(sequence)` - Cel mai lung prefix
- `checkIfDeterministic()` - Verificare DFA

---

## Observații Importante

1. **DFA vs NFA**: 
   - Verificarea secvenței și prefix-ul funcționează corect doar pentru DFA
   - Programul detectează automat dacă automatul este determinist

2. **Stare Inițială în Stări Finale**:
   - Dacă starea inițială este finală, prefixul vid (ε) este acceptat

3. **Simboluri Speciale**:
   - Spațiul se notează ca `SPACE` în fișier

4. **Comentarii**:
   - Liniile care încep cu `#` sunt ignorate
   - Liniile goale sunt ignorate

---

## Cerințe Îndeplinite

- ✅ **Cerința 1**: Citire automat finit din fișier și tastatură
- ✅ **Cerința 2**: Afișare elemente prin meniu (stări, alfabet, tranziții, stări finale)
- ✅ **Cerința 3**: Verificare secvență acceptată (DFA)
- ✅ **Cerința 4**: Cel mai lung prefix acceptat (DFA)
- ✅ **Cerința 5**: Fișier cu DFA pentru constante întregi C/C++
- ✅ **Cerința 6**: Gramatica BNF/EBNF pentru format fișier
- ✅ **Cerința 7**: Clasă pentru AF
- ✅ **Observație**: Suport pentru NFA (nedeterminist)

---

## Testare

### Test 1: DFA Simplu
```
Fișier: simple_dfa.txt
Secvențe acceptate: ab, aab, bab, aaab
Secvențe respinse: a, b, aa, ba
```

### Test 2: Constante Întregi
```
Fișier: integer_dfa.txt
Acceptate:
  - Decimal: 0, 123, -456, +789
  - Octal: 0123, 0777
  - Hexadecimal: 0x1A, 0xFF, 0X2b
Respinse: abc, 0x, -0x1, 0888 (8 nu e octal)
```

### Test 3: Prefix
```
Input: "123abc" → Prefix: "123"
Input: "-456xyz" → Prefix: "-456"
Input: "0x1Ag" → Prefix: "0x1A"
```

---

## Possible Extensions (Opțional)

1. **Conversie NFA → DFA** (algoritmul subset construction)
2. **Minimizare DFA** (algoritmul Hopcroft)
3. **Export grafic** (DOT/Graphviz)
4. **Complementul unui DFA**
5. **Uniunea/Intersecția automelor**

---

## Resurse

- **Teoria Automelor**: Introduction to Automata Theory (Hopcroft, Ullman)
- **C++ STL**: https://en.cppreference.com/
- **BNF/EBNF**: https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form

---

## Licență

Acest proiect este realizat în cadrul cursului de LFTC, UBB Cluj-Napoca.

---

**Data finalizării**: Octombrie 2024  
**Versiune**: 1.0

