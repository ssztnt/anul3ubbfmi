# Analizor Lexical cu Automate Finite - Lab 2 Partea 2
## LFTC - Limbaje Formale și Compilatoare

### Autor
Student: [Numele tău]  
Grupa: [Grupa ta]  
Anul Universitar: 2024-2025

---

## Descrierea Temei

Această tema reprezintă implementarea unui analizor lexical pentru un mini-limbaj de programare (MLP), folosind **automate finite (DFA)** pentru recunoașterea tokenilor, conform cerințelor din lab 1 partea 2.

### Cerințe Îndeplinite

1. ✅ **Adaptarea programului de analiză lexicală** pentru a folosi automate finite
2. ✅ **3 Automate Finite** implementate:
   - DFA pentru **identificatori** (pattern: `[a-zA-Z_][a-zA-Z0-9_]*`)
   - DFA pentru **constante întregi** (decimal, octal, hexadecimal)
   - DFA pentru **constante reale** (format științific: `123.456`, `1.23e10`)
3. ✅ **Fără expresii regulate** - se folosesc doar DFA-uri pure
4. ✅ **Fără spații obligatorii** - spațiile pot apărea sau nu în cod
5. ✅ **Rezultate identice cu lab 1**: FIP, TS, raportare erori

---

## Structura Proiectului

```
partea2/
├── FiniteAutomaton.h        # Header pentru clasa FiniteAutomaton
├── FiniteAutomaton.cpp      # Implementare DFA
├── LexicalAnalyzer.h        # Header pentru analizorul lexical
├── LexicalAnalyzer.cpp      # Implementare analizor
├── main.cpp                 # Program principal cu meniu
├── Makefile                 # Compilare automată
├── identifier_dfa.txt       # DFA pentru identificatori
├── integer_dfa.txt          # DFA pentru constante întregi
├── real_constant_dfa.txt    # DFA pentru constante reale
├── p1.txt                   # Program test 1 (simplu)
├── p2.txt                   # Program test 2 (constante)
├── p3.txt                   # Program test 3 (identificatori)
├── p_err.txt                # Program test cu erori
└── README.md                # Acest fișier
```

---

## Automate Finite Implementate

### 1. DFA pentru Identificatori (`identifier_dfa.txt`)

**Pattern**: `[a-zA-Z_][a-zA-Z0-9_]*`

**Descriere**: Un identificator trebuie să înceapă cu o literă (majusculă sau minusculă) sau underscore (`_`), urmat de zero sau mai multe litere, cifre sau underscore-uri.

**Exemple acceptate**:
- `x`, `myVar`, `_temp`, `count123`, `x1y2z3`

**Exemple respinse**:
- `123abc` (începe cu cifră)
- `my-var` (conține `-`)

**Stări**:
- `q0`: Stare inițială
- `q1`: Stare finală (după primul caracter valid)

**Documentație**:
- Similar cu identificatorii din C/C++
- Conform standardului ANSI C

### 2. DFA pentru Constante Întregi (`integer_dfa.txt`)

**Pattern**: Suportă multiple formate:
- **Decimal**: `[+-]?[1-9][0-9]*` sau `0`
- **Octal**: `0[0-7]+`
- **Hexadecimal**: `0[xX][0-9a-fA-F]+`

**Descriere**: Constante întregi în format C/C++, inclusiv cu semn opțional și sufixe (`L`, `LL`, `U`, `UL`, `ULL`).

**Exemple acceptate**:
- Decimal: `0`, `123`, `-456`, `+789`
- Octal: `0123`, `0777`
- Hexadecimal: `0xFF`, `0x1A`, `0X2b`

**Exemple respinse**:
- `0888` (8 nu e cifră octală)
- `0x` (hexadecimal incomplet)
- `abc` (nu e număr)

**Stări**:
- `q0`: Stare inițială
- `q1`: După semn
- `q2`: Cifre decimale (non-zero)
- `q3`: Zero (poate fi final sau început octal/hex)
- `q4`: După `0x`/`0X`
- `q5`: Cifre hexadecimale
- `q6`: Cifre octale
- (q7-q11: Stări pentru sufixe)

**Documentație**:
- Conform standardului C/C++ pentru integer literals
- Surse: ISO C Standard (ISO/IEC 9899)

### 3. DFA pentru Constante Reale (`real_constant_dfa.txt`)

**Pattern**: `[+-]?([0-9]+\.[0-9]*|\.[0-9]+)([eE][+-]?[0-9]+)?`

**Descriere**: Numere în virgulă mobilă (floating-point), inclusiv notație științifică.

**Exemple acceptate**:
- `3.14`, `-2.5`, `+1.0`
- `.456`, `123.`
- `1.23e10`, `1.5E-5`, `2.0e+3`

**Exemple respinse**:
- `123` (fără punct, e întreg)
- `.` (doar punct)
- `1.23e` (exponent incomplet)

**Stări**:
- `q0`: Stare inițială
- `q1`: După semn opțional
- `q2`: Cifre înainte de punct
- `q3`: După punct (cu cifre înainte) - **FINAL**
- `q4`: Cifre după punct - **FINAL**
- `q5`: Punct la început
- `q6`: După `e`/`E`
- `q7`: După semn în exponent
- `q8`: Cifre în exponent - **FINAL**

**Documentație**:
- Conform standardului C/C++ pentru floating-point literals
- Surse: ISO C Standard (ISO/IEC 9899)

---

## Mini-Limbaj de Programare (MLP)

### Cuvinte Cheie
```
int, float, char, string, bool
if, else, while, for, do
return, void, main
read, write
true, false
```

### Operatori
```
Aritmetici: +, -, *, /, %
Atribuire: =, +=, -=, *=, /=
Relaționali: ==, !=, <, >, <=, >=
Logici: &&, ||, !
Incrementare/Decrementare: ++, --
```

### Delimitatori
```
( ) { } [ ]
; , . :
```

---

## Compilare și Rulare

### Compilare

```bash
cd partea2
make
```

sau manual:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -o lexical_analyzer main.cpp LexicalAnalyzer.cpp FiniteAutomaton.cpp
```

### Rulare

```bash
make run
```

sau:

```bash
./lexical_analyzer
```

### Curățare

```bash
make clean      # Șterge fișierele compilate
make distclean  # Șterge tot (inclusiv output)
make rebuild    # Recompilare completă
```

---

## Utilizare

### Meniu Principal

```
1. Analizeaza fisier sursa       - Încarcă și analizează un fișier .txt
2. Analizeaza string             - Introduceți cod manual
3. Afiseaza TS                   - Tabela de Simboluri
4. Afiseaza FIP                  - Forma Internă a Programului
5. Afiseaza Erorile              - Lista de erori lexicale
6. Afiseaza toate rezultatele    - TS + FIP + Erori
7. Salveaza FIP                  - Export în fișier
8. Salveaza TS                   - Export în fișier
9. Test rapid (p1.txt)           - Analizează rapid p1.txt
0. Iesire
```

### Exemplu de Utilizare

```bash
$ ./lexical_analyzer
# Alegeți opțiunea 1
# Introduceți: p1.txt
# Alegeți opțiunea 6 (Afișare toate)
```

---

## Exemple de Test

### Test 1: Program Simplu (`p1.txt`)

**Cod:**
```c
int main() {
    int x;
    int y;
    float z;
    
    x = 10;
    y = 20;
    z = 3.14;
    
    if (x < y) {
        write(x);
    }
    
    return 0;
}
```

**Tabela de Simboluri (TS):**
```
Index   Simbol
-----   ------
0       main
1       x
2       y
3       z
4       10
5       20
6       3.14
```

**FIP (primele linii):**
```
Nr.    Cod Atom      Poz TS
---    --------      ------
0      int           0
1      ID            0
2      (             0
3      )             0
4      {             0
5      int           0
6      ID            1
7      ;             0
...
```

### Test 2: Constante (`p2.txt`)

Testează toate tipurile de constante:
- Decimal: `123`, `-456`
- Hexadecimal: `0xFF`
- Octal: `0777`
- Real: `3.14`, `-2.5`, `1.23e10`

### Test 3: Identificatori (`p3.txt`)

Testează identificatori:
- `myVar`, `_temp`, `count123`, `x1y2z3`

### Test 4: Erori (`p_err.txt`)

Testează detectarea erorilor:
- Caractere invalide: `@`, `$`
- Identificatori invalizi: `123abc` (începe cu cifră)
- Constante incomplete: `0x` (hex fără cifre)

---

## Algoritm de Analiză

### Pas 1: Tokenizare
1. Citește caracterul curent
2. Skip whitespace
3. Identifică tipul tokenului:
   - Operator (1-2 caractere)
   - Delimitator (1 caracter)
   - Token compus (citește până la separator)

### Pas 2: Clasificare Token
1. **Cuvânt cheie?** → Adaugă în FIP cu poziție 0
2. **Operator/Delimitator?** → Adaugă în FIP cu poziție 0
3. **Testare cu DFA-uri:**
   - Încearcă **DFA Real** (mai specific)
   - Încearcă **DFA Integer**
   - Încearcă **DFA Identificator**
4. **Token nerecunoscut** → Adaugă eroare

### Pas 3: Generare Rezultate
- **Tabela de Simboluri (TS)**: Identificatori și constante unice
- **FIP**: Secvență de (cod_atom, poziție_TS)
- **Erori**: Lista de erori lexicale cu linie/coloană

---

## Observații Importante

### 1. Fără Expresii Regulate
✓ **NU** se folosesc regex built-in din C++ (`<regex>`)  
✓ **SE FOLOSESC** doar automate finite pure (DFA)  
✓ Implementare manuală a recunoașterii

### 2. Spații în Cod
✓ Spațiile **NU SUNT OBLIGATORII** între tokeni  
✓ Cod valid: `int x=5;` (fără spații)  
✓ Cod valid: `int x = 5;` (cu spații)  
✓ Analizorul gestionează ambele cazuri

### 3. Priorități DFA
1. **Real** → verificat primul (mai specific: `3.14`)
2. **Integer** → al doilea (mai puțin specific: `123`)
3. **Identificator** → al treilea (pattern generic)

### 4. Gestionare Erori
- Eroare → token nerecunoscut
- Raportare cu linie și coloană exacte
- Analiza continuă după eroare (nu se oprește)

---

## Avantaje vs. Regex

| Criteriu | DFA | Regex (built-in) |
|----------|-----|------------------|
| **Claritate** | ✓ Explicit, vizual | Poate fi obscur |
| **Control** | ✓ Control total | Comportament ascuns |
| **Debugging** | ✓ Ușor de urmărit | Dificil |
| **Educație** | ✓ Învățare FA | Nu învățăm nimic |
| **Performanță** | ✓ Predictibilă | Variabilă |

---

## Teste Automate

### Rulare Teste

```bash
make test
```

### Verificare Manuală

```bash
./lexical_analyzer
# Opțiunea 9: Test rapid (p1.txt)
```

### Validare Rezultate

1. **TS** trebuie să conțină doar identificatori și constante unice
2. **FIP** trebuie să aibă o secvență corectă de (cod, poziție)
3. **Erori** pentru `p_err.txt` trebuie să detecteze toate erorile

---

## Extensii Posibile (Opțional)

1. **String literals** → DFA pentru `"text"`
2. **Caractere** → DFA pentru `'a'`
3. **Comentarii** → DFA pentru `//` și `/* */`
4. **Directivă preprocesare** → `#include`, `#define`
5. **Export grafic DFA** → Generare grafuri DOT

---

## Documentație Sursă

### Integer Literals
- **Sursa**: ISO C Standard (ISO/IEC 9899:2011) - Section 6.4.4.1
- **Descriere**: Constante întregi în baza 10, 8, 16
- **Link**: [C Standard Documentation](https://www.iso.org/standard/57853.html)

### Floating-Point Literals
- **Sursa**: ISO C Standard (ISO/IEC 9899:2011) - Section 6.4.4.2
- **Descriere**: Constante în virgulă mobilă, notație științifică
- **Link**: [C Standard Documentation](https://www.iso.org/standard/57853.html)

### Identificatori
- **Sursa**: ISO C Standard (ISO/IEC 9899:2011) - Section 6.4.2.1
- **Descriere**: Nume de variabile, funcții
- **Restricții**: Nu pot începe cu cifră

---

## Cerințe Software

- **Compilator**: g++ sau clang++ cu suport C++17
- **Sistem**: Linux, macOS, Windows (cu MinGW/WSL)
- **Make**: GNU Make (opțional, dar recomandat)

---

## Concluzie

Acest proiect implementează un **analizor lexical complet** folosind **automate finite pure**, fără a folosi expresii regulate built-in. Satisface toate cerințele din lab 2 partea 2 și produce rezultate identice cu lab 1 (FIP, TS, erori).

### Învățăminte Principale

1. ✅ Automate finite pentru recunoaștere pattern-uri
2. ✅ Construcție DFA pentru identificatori, întregi, reale
3. ✅ Integrare DFA în analizor lexical
4. ✅ Gestionare erori lexicale
5. ✅ Generare FIP și TS

---

**Data finalizării**: Noiembrie 2024  
**Versiune**: 2.0

