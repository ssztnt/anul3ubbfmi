#!/usr/bin/env python3
"""
Verification script to compare MPI results with sequential results
"""

def normalize_content(filename):
    """Read file and normalize whitespace"""
    with open(filename, 'r') as f:
        content = f.read()
    # Remove all whitespace and compare just the digits
    return ''.join(content.split())

def compare_files(ref_file, test_file):
    """Compare two result files"""
    ref = normalize_content(ref_file)
    test = normalize_content(test_file)
    return ref == test

def main():
    print("\n========== RESULT VERIFICATION ==========")
    
    tests = [
        ("Variant 1 (Standard)", "result.txt", "result1.txt"),
        ("Variant 2 (Scatter)", "result.txt", "resultScatter.txt"),
        ("Variant 3 (Async)", "result.txt", "resultAsync.txt"),
        ("Variant 1.1 (Optimized)", "result.txt", "resultOptimized.txt"),
    ]
    
    for name, ref, test in tests:
        try:
            if compare_files(ref, test):
                print(f"[OK] {name} matches sequential result")
            else:
                print(f"[FAIL] {name} does NOT match sequential result!")
                print(f"  Reference: {normalize_content(ref)[:50]}...")
                print(f"  Test: {normalize_content(test)[:50]}...")
        except FileNotFoundError as e:
            print(f"[ERROR] {name}: File not found - {e}")
    
    print("=========================================\n")

if __name__ == "__main__":
    main()

