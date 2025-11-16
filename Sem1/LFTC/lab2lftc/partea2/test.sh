#!/bin/bash

# Test script pentru analizorul lexical

echo "======================================"
echo "  TEST ANALIZOR LEXICAL - Lab 2 Partea 2"
echo "======================================"
echo ""

cd /Users/plaiurares/Documents/Facultate/anul3ubbfmi/Sem1/LFTC/lab2lftc/partea2

# Test 1: Program simplu
echo "=== Test 1: Program simplu (p1.txt) ==="
echo "9" | ./lexical_analyzer
echo ""

# Test 2: DFA identificatori
echo "=== Test 2: Test DFA Identificatori ==="
echo "Testare: myVar, _temp, count123"
echo ""

# Test 3: DFA constante intregi
echo "=== Test 3: Test DFA Constante Intregi ==="
echo "Testare: 123, -456, 0xFF, 0777"
echo ""

# Test 4: DFA constante reale
echo "=== Test 4: Test DFA Constante Reale ==="
echo "Testare: 3.14, -2.5, 1.23e10"
echo ""

echo "======================================"
echo "  TESTE FINALIZATE"
echo "======================================"

