# MPI Large Number Addition - Lab 3

## Project Overview
This project implements parallel addition of large numbers (>10 digits) using MPI (Message Passing Interface). The implementation includes multiple variants with different communication patterns and optimizations.

## Variants Implemented

### ✅ Variant 0: Sequential (Reference)
- **File:** `SequentialCalculation.cpp`
- **Output:** `result.txt`
- **Description:** Standard sequential addition algorithm used as reference for correctness verification

### ✅ Variant 1: Standard Communication
- **File:** `StandardCalculation.cpp`
- **Output:** `result1.txt`
- **Description:** Uses `MPI_Send` and `MPI_Recv` for communication
- **Key Features:**
  - Process 0 distributes digit chunks to worker processes
  - Workers compute partial sums and pass carry to next process
  - Results aggregated sequentially in process 0
  - Can be tested with `MPI_Ssend` for deadlock detection

### ✅ Variant 2: Scatter/Gather
- **File:** `ScatterCalculation.cpp`
- **Output:** `resultScatter.txt`
- **Description:** Uses collective operations `MPI_Scatter` and `MPI_Gather`
- **Key Features:**
  - Process 0 reads both numbers and pads to P-divisible length
  - Digits distributed evenly using MPI_Scatter
  - All processes compute their portion simultaneously
  - Results collected using MPI_Gather

### ✅ Variant 3: Asynchronous Communication
- **File:** `AsyncCalculation.cpp`
- **Output:** `resultAsync.txt`
- **Description:** Uses `MPI_Isend` and `MPI_Irecv` for non-blocking communication
- **Key Features:**
  - Process 0 sends data asynchronously to all workers
  - Workers use MPI_Irecv to receive data from process 0
  - Carry propagation handled with proper synchronization
  - Results sent back asynchronously to process 0

### ✅ Variant 1.1: Optimized Standard (BONUS)
- **File:** `OptimizedCalculation.cpp`
- **Output:** `resultOptimized.txt`
- **Description:** Optimization of Variant 1 - processes start computing before receiving carry
- **Key Features:**
  - Workers begin addition immediately upon receiving digits
  - Carry from previous process applied after initial computation
  - Reduces idle waiting time
  - Worth 2 bonus points

## Project Structure

```
tema3ppd/
├── header/
│   ├── AsyncCalculation.h
│   ├── GenerateNumber.h
│   ├── OptimizedCalculation.h
│   ├── ScatterCalculation.h
│   ├── SequentialCalculation.h
│   ├── StandardCalculation.h
│   └── Verification.h
├── source/
│   ├── AsyncCalculation.cpp
│   ├── GenerateNumber.cpp
│   ├── OptimizedCalculation.cpp
│   ├── ScatterCalculation.cpp
│   ├── SequentialCalculation.cpp
│   ├── StandardCalculation.cpp
│   └── Verification.cpp
├── main.cpp
├── Makefile
├── CMakeLists.txt
├── test_all.sh
├── verify.py
└── README.md
```

## Output Files

| Variant | Output File | Description |
|---------|------------|-------------|
| **Variant 0** | `result.txt` | Sequential reference result |
| **Variant 1** | `result1.txt` | Standard MPI communication |
| **Variant 2** | `resultScatter.txt` | Scatter/Gather collective operations |
| **Variant 3** | `resultAsync.txt` | Asynchronous communication |
| **Variant 1.1** | `resultOptimized.txt` | Optimized standard communication |

### Input Files
- `firstNumber.txt` - First large number (auto-generated)
- `secondNumber.txt` - Second large number (auto-generated)

### File Format
All files contain:
- First line (optional): Number of digits
- Following: Space-separated digits in **little-endian** format (least significant digit first)

Example for number 123:
```
3 2 1
```

## Building the Project

### Using Make (Recommended)
```bash
make clean
make
```

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Interactive Menu Mode
```bash
mpirun -np <P> ./Tema_3 <N1> <N2>
```

Example:
```bash
mpirun -np 5 ./Tema_3 100 100
```

This will show an interactive menu:
```
========================================
  MPI LARGE NUMBER ADDITION
========================================
Select variant to run:
  0 - Sequential (reference)
  1 - Variant 1: Standard Communication
  2 - Variant 2: Scatter/Gather
  3 - Variant 3: Asynchronous Communication
  4 - Variant 1.1: Optimized
  5 - Run ALL variants + verification
  6 - Verification only
========================================
Choice:
```

### Command Line Mode
```bash
mpirun -np <P> ./Tema_3 <N1> <N2> <variant>
```

Examples:
```bash
# Run Variant 1 with 100 digits, 5 processes
mpirun --oversubscribe -np 5 ./Tema_3 100 100 1

# Run all variants with verification
mpirun --oversubscribe -np 5 ./Tema_3 1000 1000 5

# Run Scatter/Gather with 4 processes
mpirun --oversubscribe -np 4 ./Tema_3 16 16 2
```

### Parameters
- `N1` - Number of digits in first number
- `N2` - Number of digits in second number
- `P` - Number of MPI processes
- `variant` (optional) - Which variant to run (0-6)

## Testing

### Manual Testing
Run individual test cases:
```bash
# Test with 16 digits, 5 processes
mpirun --oversubscribe -np 5 ./Tema_3 16 16 5

# Test with different sized numbers
mpirun --oversubscribe -np 5 ./Tema_3 100 10000 5

# Test Scatter/Gather with 4 processes
mpirun --oversubscribe -np 4 ./Tema_3 1000 1000 2
```

### Automated Testing
Run the comprehensive test suite:
```bash
./test_all.sh
```

This runs all required test cases:
- 16 digits with 4/5 processes
- 1000/10000 digits with various process counts
- Different sized numbers (100 vs 100000 digits)

### Verification
Verify correctness of existing results:
```bash
mpirun -np 2 ./Tema_3 100 100 6
# OR
python3 verify.py
```

## Required Test Cases (from specification)

### Variant 1 & 3 Tests:
- ✅ N₁=N₂=16, 5 processes
- ✅ N₁=N₂=10000, 5 processes
- ✅ N₁=N₂=10000, 9 processes
- ✅ N₁=N₂=10000, 17 processes
- ✅ N₁=100, N₂=100000, 5 processes
- ✅ N₁=100, N₂=100000, 9 processes
- ✅ N₁=100, N₂=100000, 17 processes

### Variant 2 Tests:
- ✅ N₁=N₂=16, 4 processes
- ✅ N₁=N₂=1000, 4 processes
- ✅ N₁=N₂=1000, 8 processes
- ✅ N₁=N₂=1000, 16 processes
- ✅ N₁=100, N₂=100000, 4 processes
- ✅ N₁=100, N₂=100000, 8 processes
- ✅ N₁=100, N₂=100000, 16 processes

## Implementation Details

### Number Representation
- Numbers stored as arrays of integers
- Little-endian: least significant digit at index 0
- Example: 123 → `[3, 2, 1]`

### Communication Patterns

**Variant 1 (Standard):**
1. Process 0 distributes digit chunks sequentially
2. Workers compute local sums
3. Carry propagates sequentially (process i → process i+1)
4. Results sent back to process 0

**Variant 2 (Scatter/Gather):**
1. Process 0 reads and pads numbers
2. MPI_Scatter distributes equal chunks
3. All processes compute simultaneously
4. Carry propagation after local computation
5. MPI_Gather collects results

**Variant 3 (Async):**
1. Process 0 initiates all sends with MPI_Isend
2. Workers use MPI_Irecv for non-blocking receives
3. MPI_Wait ensures data arrival before computation
4. Carry handling with proper synchronization
5. Results sent back with MPI_Isend

**Variant 1.1 (Optimized):**
1. Same as Variant 1 but workers start computing immediately
2. Carry applied after initial computation completes
3. Reduces waiting time compared to standard approach

### Deadlock Prevention
- Communication designed to avoid circular dependencies
- Tested with `MPI_Ssend` (synchronous send) to verify correctness
- Proper ordering: data transmission → computation → carry propagation

## Verification

The project includes automatic verification:
- Compares all MPI variant outputs against sequential result
- Normalizes whitespace for comparison
- Reports success/failure for each variant

All variants must produce identical results to pass verification.

## Performance Considerations

- Process 0 doesn't compute sums (coordinates only)
- Work distributed among P-1 worker processes
- File I/O included in timing measurements
- Carry propagation is sequential bottleneck

## Requirements Met

✅ All variants implemented (0, 1, 2, 3, 1.1)
✅ C++11+ implementation  
✅ Correct number representation (LSB at position 0)  
✅ MPI communication working correctly  
✅ No deadlocks  
✅ Result verification system  
✅ All test cases passing  
✅ Optimization variant (1.1) for bonus points  

## Notes

- Process 0 handles file I/O and coordination
- Worker processes (1 to P-1) perform computations
- Carry must be handled correctly for accurate results
- For Scatter/Gather: if N is not divisible by P, padding with zeros is used

## Author
PPD Laboratory 3 - MPI Large Number Addition

