#!/bin/bash

# Test script for MPI Large Number Addition
# This script runs all required test cases from the lab specification

echo "======================================"
echo "MPI Large Number Addition Test Suite"
echo "======================================"
echo ""

# Build the project
echo "Building project..."
mkdir -p build
cd build
cmake ..
make
cd ..

if [ ! -f "build/Tema_3" ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build successful!"
echo ""

# Function to run a test
run_test() {
    local N1=$1
    local N2=$2
    local PROCS=$3
    local TEST_NAME=$4
    
    echo "----------------------------------------"
    echo "Test: $TEST_NAME"
    echo "N1=$N1, N2=$N2, Processes=$PROCS"
    echo "----------------------------------------"
    
    mpirun -np $PROCS ./build/Tema_3 $N1 $N2
    
    if [ $? -eq 0 ]; then
        echo "✓ Test completed successfully"
    else
        echo "✗ Test failed"
    fi
    echo ""
}

# Test Set 1: Variant 1 (Standard) and Variant 3 (Async)
echo "=== Test Set 1: Standard & Async Communication ==="
echo ""

# Test 1.1
run_test 16 16 5 "16 digits + 16 digits, 5 processes"

# Test 1.2
run_test 10000 10000 5 "10000 digits + 10000 digits, 5 processes"

# Test 1.3
run_test 10000 10000 9 "10000 digits + 10000 digits, 9 processes"

# Test 1.4
run_test 10000 10000 17 "10000 digits + 10000 digits, 17 processes"

# Test 1.5
run_test 100 100000 5 "100 digits + 100000 digits, 5 processes"

# Test 1.6
run_test 100 100000 9 "100 digits + 100000 digits, 9 processes"

# Test 1.7
run_test 100 100000 17 "100 digits + 100000 digits, 17 processes"

echo ""
echo "=== Test Set 2: Scatter/Gather ==="
echo ""

# Test 2.1
run_test 16 16 4 "16 digits + 16 digits, 4 processes"

# Test 2.2
run_test 1000 1000 4 "1000 digits + 1000 digits, 4 processes"

# Test 2.3
run_test 1000 1000 8 "1000 digits + 1000 digits, 8 processes"

# Test 2.4
run_test 1000 1000 16 "1000 digits + 1000 digits, 16 processes"

# Test 2.5
run_test 100 100000 4 "100 digits + 100000 digits, 4 processes"

# Test 2.6
run_test 100 100000 8 "100 digits + 100000 digits, 8 processes"

# Test 2.7
run_test 100 100000 16 "100 digits + 100000 digits, 16 processes"

echo "======================================"
echo "All tests completed!"
echo "======================================"
echo ""
echo "Check the following result files:"
echo "  - result.txt (Sequential - reference)"
echo "  - result1.txt (Variant 1 - Standard)"
echo "  - resultScatter.txt (Variant 2 - Scatter/Gather)"
echo "  - resultAsync.txt (Variant 3 - Async)"
echo "  - resultOptimized.txt (Variant 1.1 - Optimized)"

