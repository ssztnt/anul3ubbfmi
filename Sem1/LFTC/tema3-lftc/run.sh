#!/bin/bash

# Script pentru rulare rapida - Tema 3 LFTC

echo "========================================="
echo "   ANALIZOR LEXICAL - FLEX - Tema 3"
echo "========================================="
echo ""

# Compilare
echo "1. Compilare analizor..."
make clean > /dev/null 2>&1
make all

if [ $? -ne 0 ]; then
    echo "✗ Eroare la compilare!"
    exit 1
fi

echo ""
echo "========================================="
echo "2. Testare pe toate fisierele"
echo "========================================="

# Testare fiecare fisier
for file in p1.txt p2.txt p3.txt p_err.txt; do
    if [ -f "$file" ]; then
        echo ""
        echo ">>> Testare: $file"
        echo "----------------------------------------"
        ./lexer "$file"
        echo ""
        read -p "Apasa ENTER pentru urmatorul test..."
    fi
done

echo ""
echo "========================================="
echo "✓ Testare completa!"
echo "========================================="

