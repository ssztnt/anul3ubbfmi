# Documentație Automate Finite - Lab 2 Partea 2

## 1. DFA pentru Identificatori

### Definiție Formală

**M_ID = (Q, Σ, δ, q₀, F)**

Unde:
- **Q** = {q₀, q₁} - Mulțimea stărilor
- **Σ** = {a-z, A-Z, _, 0-9} - Alfabetul
- **q₀** = Starea inițială
- **F** = {q₁} - Mulțimea stărilor finale
- **δ** - Funcția de tranziție:

```
δ(q₀, [a-zA-Z_]) = q₁     # Prima literă sau underscore
δ(q₁, [a-zA-Z0-9_]) = q₁  # Continuare cu litere, cifre, underscore
```

### Pattern Recunoscut

```
ID ::= [a-zA-Z_][a-zA-Z0-9_]*
```

### Exemple

**Acceptate:**
- `x`, `myVar`, `_temp`, `count123`, `x1y2z3`, `__init__`

**Respinse:**
- `123abc` (începe cu cifră)
- `my-var` (conține `-`)
- `my var` (conține spațiu)

### Diagrama Stări

```
     [a-zA-Z_]        [a-zA-Z0-9_]
(q₀) ─────────> ((q₁)) ─────────┐
                  ^              │
                  └──────────────┘
```

### Observații

- Identificatorii **nu pot începe cu cifră**
- Pot conține underscore în orice poziție
- Case-sensitive: `myVar` ≠ `myvar`

---

## 2. DFA pentru Constante Întregi

### Definiție Formală

**M_INT = (Q, Σ, δ, q₀, F)**

Unde:
- **Q** = {q₀, q₁, q₂, q₃, q₄, q₅, q₆, q₇, q₈, q₉, q₁₀, q₁₁}
- **Σ** = {0-9, +, -, x, X, a-f, A-F, u, U, l, L}
- **q₀** = Starea inițială
- **F** = {q₂, q₃, q₅, q₆, q₇, q₈, q₉, q₁₀, q₁₁}
- **δ** - Funcția de tranziție (vezi fișier `integer_dfa.txt`)

### Pattern-uri Recunoscute

#### 2.1 Decimal (baza 10)
```
DECIMAL ::= [+-]?[1-9][0-9]* | 0
```

**Exemple:**
- `0`, `123`, `+456`, `-789`

#### 2.2 Octal (baza 8)
```
OCTAL ::= 0[0-7]+
```

**Exemple:**
- `0123`, `0777`, `0644`

**Invalid:**
- `0888` (8 nu e cifră octală)

#### 2.3 Hexadecimal (baza 16)
```
HEX ::= 0[xX][0-9a-fA-F]+
```

**Exemple:**
- `0xFF`, `0x1A`, `0X2b`, `0xDEADBEEF`

**Invalid:**
- `0x` (fără cifre)
- `0xG` (G nu e cifră hex)

#### 2.4 Cu Sufixe (opțional)
```
SUFFIX ::= [uU]?[lL]{0,2}
```

**Exemple:**
- `123L`, `456LL`, `789U`, `0xFFUL`, `100ULL`

### Diagrama Stări (Simplificată)

```
        +/-         1-9         0-9
(q₀) ─────> (q₁) ─────> ((q₂)) ─────┐
  │                                  │
  │ 0                                │
  └────> ((q₃)) ─────────────────────┘
            │
            │ x/X
            └────> (q₄) ──0-9a-f──> ((q₅))
            │                         │
            │ 0-7                     │
            └────> ((q₆)) ────────────┘
```

### Documentație Sursă

**Standard:** ISO C (ISO/IEC 9899:2011) - Section 6.4.4.1  
**Titlu:** Integer constants  
**Descriere:** Constante întregi literale în C/C++

**Link:** [C Standard - Integer Constants](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)

---

## 3. DFA pentru Constante Reale

### Definiție Formală

**M_REAL = (Q, Σ, δ, q₀, F)**

Unde:
- **Q** = {q₀, q₁, q₂, q₃, q₄, q₅, q₆, q₇, q₈}
- **Σ** = {0-9, +, -, ., e, E}
- **q₀** = Starea inițială
- **F** = {q₃, q₄, q₈} - Stări finale
- **δ** - Funcția de tranziție (vezi fișier `real_constant_dfa.txt`)

### Pattern Recunoscut

```
REAL ::= [+-]? ( [0-9]+ \. [0-9]* | \. [0-9]+ ) ([eE][+-]?[0-9]+)?
```

### Componente

#### 3.1 Parte Întreagă + Fracționară
```
SIMPLE ::= [0-9]+\.[0-9]* | \.[0-9]+
```

**Exemple:**
- `3.14`, `123.456`, `.5`, `123.`

#### 3.2 Notație Științifică (Opțional)
```
EXPONENT ::= [eE][+-]?[0-9]+
```

**Exemple complete:**
- `1.23e10` = 1.23 × 10¹⁰
- `2.5E-3` = 2.5 × 10⁻³
- `1.0e+5` = 1.0 × 10⁵

### Diagrama Stări

```
       +/-        0-9         .          0-9
(q₀) ─────> (q₁) ───> (q₂) ───> ((q₃)) ───> ((q₄))
  │                                │           │
  │ .                              │e/E        │e/E
  └───> (q₅) ──0-9──> ((q₄))      └──> (q₆) <─┘
                                         │+/-
                                         └──> (q₇) ──0-9──> ((q₈))
```

### Exemple Detaliate

#### Acceptate
| Input | Interpretare |
|-------|--------------|
| `3.14` | Pi aproximat |
| `0.5` | Jumătate |
| `.5` | Jumătate (notație alternativă) |
| `123.` | 123.0 |
| `1.23e10` | 12,300,000,000 |
| `2.5E-3` | 0.0025 |
| `-1.5` | Negativ |

#### Respinse
| Input | Motiv |
|-------|-------|
| `123` | Fără punct (e întreg) |
| `.` | Doar punct |
| `1.23e` | Exponent incomplet |
| `e10` | Fără mantisă |

### Documentație Sursă

**Standard:** ISO C (ISO/IEC 9899:2011) - Section 6.4.4.2  
**Titlu:** Floating constants  
**Descriere:** Constante în virgulă mobilă în C/C++

**Link:** [C Standard - Floating Constants](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)

---

## Verificare Formală

### Proprietăți Matematice

#### 1. Determinism
Toate cele 3 DFA-uri sunt **deterministe**:
- ∀ q ∈ Q, ∀ a ∈ Σ: |δ(q, a)| ≤ 1

#### 2. Completitudine
DFA-urile nu sunt complete (nu au tranziții pentru toate simbolurile):
- Dacă δ(q, a) = ∅ → Token invalid

#### 3. Minimalitate
DFA-urile folosesc numărul minim de stări necesare pentru recunoaștere.

---

## Algoritm de Recunoaștere

### Pseudocod

```python
def recognize(dfa, input):
    state = dfa.initial_state
    
    for symbol in input:
        if symbol not in dfa.alphabet:
            return False  # Simbol invalid
        
        next_state = dfa.transition(state, symbol)
        
        if next_state is None:
            return False  # Nu există tranziție
        
        state = next_state
    
    return state in dfa.final_states
```

### Complexitate

- **Timp:** O(n) unde n = lungimea input-ului
- **Spațiu:** O(1) (doar starea curentă)

---

## Integrare în Analizor

### Ordinea Verificării

1. **Verifică cuvinte cheie** (direct în set)
2. **Verifică operatori** (direct în set)
3. **Verifică delimitatori** (direct în set)
4. **DFA Real** (mai specific: necesită `.`)
5. **DFA Integer** (mai puțin specific)
6. **DFA Identificator** (cel mai general)

### Justificare Ordine

- **Real înainte de Integer** pentru a evita:
  - `3.14` → `3` (integer) + `.14` (error)
  - Corect: `3.14` → real

---

## Teste Unitare

### Test DFA Identificatori

```cpp
ASSERT_TRUE(identifierFA.isSequenceAccepted("myVar"));
ASSERT_TRUE(identifierFA.isSequenceAccepted("_temp"));
ASSERT_FALSE(identifierFA.isSequenceAccepted("123abc"));
```

### Test DFA Integer

```cpp
ASSERT_TRUE(integerFA.isSequenceAccepted("123"));
ASSERT_TRUE(integerFA.isSequenceAccepted("-456"));
ASSERT_TRUE(integerFA.isSequenceAccepted("0xFF"));
ASSERT_TRUE(integerFA.isSequenceAccepted("0777"));
ASSERT_FALSE(integerFA.isSequenceAccepted("0x"));
```

### Test DFA Real

```cpp
ASSERT_TRUE(realFA.isSequenceAccepted("3.14"));
ASSERT_TRUE(realFA.isSequenceAccepted("1.23e10"));
ASSERT_FALSE(realFA.isSequenceAccepted("123"));
ASSERT_FALSE(realFA.isSequenceAccepted("."));
```

---

## Referințe

1. **Hopcroft, J.E., Ullman, J.D.** (2006). *Introduction to Automata Theory, Languages, and Computation*. 3rd Edition.

2. **ISO/IEC 9899:2011** - Programming languages — C

3. **Dragon Book** - Compilers: Principles, Techniques, and Tools (Aho, Lam, Sethi, Ullman)

4. **C Standard Documentation**
   - Integer literals: Section 6.4.4.1
   - Floating-point literals: Section 6.4.4.2
   - Identifiers: Section 6.4.2.1

---

**Autor:** [Numele tău]  
**Data:** Noiembrie 2024  
**Versiune:** 1.0

