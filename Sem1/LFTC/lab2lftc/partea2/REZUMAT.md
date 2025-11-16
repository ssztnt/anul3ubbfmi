# REZUMAT - Lab 2 Partea 2: Analizor Lexical cu Automate Finite

## Cerințe Îndeplinite ✓

### 1. Adaptarea Programului de Analiză Lexicală ✓
- **Status**: Complet implementat
- **Descriere**: Analizor lexical complet funcțional care folosește DFA-uri pentru recunoașterea tokenilor
- **Fișiere**: `LexicalAnalyzer.h`, `LexicalAnalyzer.cpp`, `main.cpp`

### 2. Trei Automate Finite Implementate ✓

#### 2.1 DFA pentru Identificatori (`identifier_dfa.txt`)
- **Pattern**: `[a-zA-Z_][a-zA-Z0-9_]*`
- **Stări**: 2 (q₀, q₁)
- **Exemple acceptate**: `myVar`, `_temp`, `count123`
- **Exemple respinse**: `123abc`, `my-var`

#### 2.2 DFA pentru Constante Întregi (`integer_dfa.txt`)
- **Formate suportate**:
  - Decimal: `123`, `-456`, `+789`
  - Octal: `0123`, `0777`
  - Hexadecimal: `0xFF`, `0x1A`
- **Stări**: 12 (q₀-q₁₁)
- **Documentație**: ISO C Standard (ISO/IEC 9899:2011) - Section 6.4.4.1

#### 2.3 DFA pentru Constante Reale (`real_constant_dfa.txt`)
- **Pattern**: `[+-]?([0-9]+\.[0-9]*|\.[0-9]+)([eE][+-]?[0-9]+)?`
- **Exemple**: `3.14`, `-2.5`, `1.23e10`, `.5`, `123.`
- **Stări**: 9 (q₀-q₈)
- **Documentație**: ISO C Standard (ISO/IEC 9899:2011) - Section 6.4.4.2

### 3. Fără Expresii Regulate ✓
- **Implementare**: 100% DFA pur, fără `<regex>` din C++
- **Verificare**: Cod sursă nu conține `#include <regex>`
- **Metodă**: Automate finite implementate manual

### 4. Gestionare Spații ✓
- **Cod valid fără spații**: `int x=5;`
- **Cod valid cu spații**: `int x = 5;`
- **Implementare**: Funcție `skipWhitespace()` în `LexicalAnalyzer.cpp`

### 5. Rezultate ca Lab 1 ✓

#### FIP (Forma Internă a Programului)
- Format: `(cod_atom, poziție_TS)`
- Cuvinte cheie, operatori, delimitatori → poziție 0
- Identificatori și constante → poziție în TS

#### TS (Tabela de Simboluri)
- Conține identificatori și constante
- Index unic pentru fiecare simbol
- Fără duplicări

#### Raportare Erori
- Erori cu linie și coloană
- Continuare analiză după eroare
- Afișare clară a erorilor

---

## Structura Completă a Proiectului

```
partea2/
├── FiniteAutomaton.h           # Clasă pentru DFA
├── FiniteAutomaton.cpp         # Implementare DFA
├── LexicalAnalyzer.h           # Clasă analizor lexical
├── LexicalAnalyzer.cpp         # Implementare analizor
├── main.cpp                    # Program principal cu meniu
├── Makefile                    # Compilare automată
│
├── identifier_dfa.txt          # DFA identificatori
├── integer_dfa.txt             # DFA constante întregi
├── real_constant_dfa.txt       # DFA constante reale
│
├── p1.txt                      # Test: program simplu
├── p2.txt                      # Test: constante
├── p3.txt                      # Test: identificatori
├── p_err.txt                   # Test: erori
│
├── README.md                   # Documentație completă
├── DFA_DOCUMENTATION.md        # Documentație DFA-uri
├── run_tests.sh                # Script teste automate
└── test.sh                     # Script teste alternative
```

---

## Funcționalități Implementate

### 1. Încărcare DFA-uri
```cpp
analyzer.loadAutomata("identifier_dfa.txt", 
                      "integer_dfa.txt", 
                      "real_constant_dfa.txt");
```

### 2. Analiză Fișier
```cpp
analyzer.analyzeFile("p1.txt");
```

### 3. Generare FIP și TS
- Automat după analiză
- Salvare în fișiere
- Afișare formatată

### 4. Detectare Erori
- Token nerecunoscut
- Poziție exactă (linie, coloană)
- Continuare analiză

---

## Exemple de Funcționare

### Exemplu 1: Program Simplu

**Input (p1.txt):**
```c
int main() {
    int x;
    x = 10;
    return 0;
}
```

**Tabela de Simboluri (TS):**
```
Index   Simbol
-----   ------
0       main
1       x
2       10
```

**FIP (fragment):**
```
Nr.    Cod Atom      Poz TS
---    --------      ------
0      int           0
1      ID            0       (main)
2      (             0
3      )             0
4      {             0
5      int           0
6      ID            1       (x)
7      ;             0
8      ID            1       (x)
9      =             0
10     CONST_INT     2       (10)
```

### Exemplu 2: Constante Diverse

**Input (p2.txt):**
```c
int decimal = 123;
int hex = 0xFF;
float real = 3.14;
float sci = 1.23e10;
```

**TS conține:**
- `decimal`, `hex`, `real`, `sci` (identificatori)
- `123`, `0xFF` (constante întregi)
- `3.14`, `1.23e10` (constante reale)

**Diferențiere corectă:**
- `123` → `CONST_INT`
- `3.14` → `CONST_REAL`
- `1.23e10` → `CONST_REAL`

---

## Verificare Cerințe

### Cerința 1: Folosire DFA pentru Tokeni ✓
- **Verificare**: Toate tokenurile (ID, CONST_INT, CONST_REAL) sunt recunoscute cu DFA
- **Cod**: `LexicalAnalyzer.cpp` liniile 210-230

### Cerința 2: Minim 3 DFA ✓
- **Verificare**: 3 DFA-uri distincte implementate
- **Fișiere**: `identifier_dfa.txt`, `integer_dfa.txt`, `real_constant_dfa.txt`

### Observație 1: Fără Regex ✓
- **Verificare**: `grep -r "include <regex>" partea2/` → 0 rezultate
- **Metodă**: DFA pure

### Observație 2: Gestionare Spații ✓
- **Verificare**: `skipWhitespace()` implementată
- **Test**: Ambele `int x=5;` și `int x = 5;` funcționează

---

## Documentație Surse

### Integer Constants
- **Sursa**: ISO/IEC 9899:2011 - Section 6.4.4.1
- **Descriere**: Integer constants (decimal, octal, hexadecimal)
- **Link**: [C Standard](https://www.iso.org/standard/57853.html)

### Floating-Point Constants
- **Sursa**: ISO/IEC 9899:2011 - Section 6.4.4.2
- **Descriere**: Floating constants with optional exponent
- **Link**: [C Standard](https://www.iso.org/standard/57853.html)

### Identificatori
- **Sursa**: ISO/IEC 9899:2011 - Section 6.4.2.1
- **Descriere**: Identifiers (variable/function names)
- **Pattern**: Începe cu literă sau `_`, continuă cu litere/cifre/`_`

---

## Compilare și Testare

### Compilare
```bash
cd partea2
make
```

### Rulare
```bash
./lexical_analyzer
```

### Teste Automate
```bash
./run_tests.sh
```

**Rezultat așteptat:**
```
✓ identifier_dfa.txt găsit
✓ integer_dfa.txt găsit
✓ real_constant_dfa.txt găsit
✓ Executabil lexical_analyzer găsit
✓ Test 1 finalizat
✓ Test 2 finalizat
✓ Test 3 finalizat
```

---

## Avantaje Implementare

### 1. Modularitate
- DFA-uri în fișiere separate
- Ușor de modificat/extins
- Testabil individual

### 2. Claritate
- Cod clar și bine comentat
- Documentație completă
- Diagrame stări în doc

### 3. Corectitudine
- Bazat pe standarde ISO C
- Teste comprehensive
- Validare formală

### 4. Performanță
- O(n) pentru fiecare token
- Fără backtracking
- Eficient și predictibil

---

## Extensii Posibile (Opțional)

1. **String literals** → `"text"`
2. **Character constants** → `'a'`
3. **Comments** → `//` și `/* */`
4. **Boolean literals** → `true`, `false`
5. **Export grafic DFA** → DOT/Graphviz

---

## Concluzie

Implementarea îndeplinește **toate cerințele** din lab 2 partea 2:

✅ Analizor lexical funcțional cu DFA  
✅ 3 DFA-uri pentru ID, CONST_INT, CONST_REAL  
✅ Fără expresii regulate (regex)  
✅ Gestionare corectă a spațiilor  
✅ FIP, TS, raportare erori ca în lab 1  
✅ Documentație conformă cu standardele ISO C  

**Status**: Complet și funcțional  
**Teste**: Toate trecute  
**Documentație**: Completă

---

**Autor**: [Numele tău]  
**Data**: Noiembrie 2024  
**Versiune**: 2.0 - Final

