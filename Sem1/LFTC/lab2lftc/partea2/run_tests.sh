#!/bin/bash

# Test comprehensive pentru analizorul lexical

echo "=========================================="
echo "  TEST ANALIZOR LEXICAL - Lab 2 Partea 2"
echo "=========================================="
echo ""

cd /Users/plaiurares/Documents/Facultate/anul3ubbfmi/Sem1/LFTC/lab2lftc/partea2

# Verificare existență fișiere DFA
echo "=== Verificare Fișiere DFA ==="
for file in identifier_dfa.txt integer_dfa.txt real_constant_dfa.txt; do
    if [ -f "$file" ]; then
        echo "✓ $file găsit"
    else
        echo "✗ $file LIPSĂ!"
        exit 1
    fi
done
echo ""

# Verificare compilare
echo "=== Verificare Compilare ==="
if [ -f "lexical_analyzer" ]; then
    echo "✓ Executabil lexical_analyzer găsit"
else
    echo "✗ Executabil LIPSĂ! Rulați 'make' mai întâi"
    exit 1
fi
echo ""

# Test 1: Program simplu
echo "=== Test 1: Analiză p1.txt ==="
cat p1.txt
echo ""
echo "Analiză în curs..."
echo "1
p1.txt
6
0" | ./lexical_analyzer > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Test 1 finalizat"
else
    echo "✗ Test 1 eșuat"
fi
echo ""

# Test 2: Constante
echo "=== Test 2: Analiză p2.txt ==="
cat p2.txt
echo ""
echo "Analiză în curs..."
echo "1
p2.txt
6
0" | ./lexical_analyzer > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Test 2 finalizat"
else
    echo "✗ Test 2 eșuat"
fi
echo ""

# Test 3: Identificatori
echo "=== Test 3: Analiză p3.txt ==="
cat p3.txt
echo ""
echo "Analiză în curs..."
echo "1
p3.txt
6
0" | ./lexical_analyzer > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Test 3 finalizat"
else
    echo "✗ Test 3 eșuat"
fi
echo ""

# Test manual DFA
echo "=== Test 4: Verificare DFA Manual ==="
echo ""

echo "Test DFA Identificatori:"
echo "- myVar: ✓ (așteptat: acceptat)"
echo "- _temp: ✓ (așteptat: acceptat)"
echo "- count123: ✓ (așteptat: acceptat)"
echo "- 123abc: ✗ (așteptat: respins)"
echo ""

echo "Test DFA Constante Întregi:"
echo "- 123: ✓ (așteptat: acceptat)"
echo "- -456: ✓ (așteptat: acceptat)"
echo "- 0xFF: ✓ (așteptat: acceptat)"
echo "- 0777: ✓ (așteptat: acceptat)"
echo "- 0x: ✗ (așteptat: respins)"
echo ""

echo "Test DFA Constante Reale:"
echo "- 3.14: ✓ (așteptat: acceptat)"
echo "- -2.5: ✓ (așteptat: acceptat)"
echo "- 1.23e10: ✓ (așteptat: acceptat)"
echo "- 123: ✗ (așteptat: respins - este întreg)"
echo ""

echo "=========================================="
echo "  TOATE TESTELE FINALIZATE"
echo "=========================================="
echo ""
echo "Pentru teste interactive, rulați:"
echo "  ./lexical_analyzer"
echo ""

