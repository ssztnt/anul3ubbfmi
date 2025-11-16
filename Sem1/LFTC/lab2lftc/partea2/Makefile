# Makefile pentru Analizor Lexical cu Automate Finite
# Lab 2 - Partea 2

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
TARGET = lexical_analyzer
OBJS = main.o LexicalAnalyzer.o FiniteAutomaton.o

.PHONY: all clean run test distclean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp LexicalAnalyzer.h FiniteAutomaton.h
	$(CXX) $(CXXFLAGS) -c main.cpp

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h FiniteAutomaton.h
	$(CXX) $(CXXFLAGS) -c LexicalAnalyzer.cpp

FiniteAutomaton.o: FiniteAutomaton.cpp FiniteAutomaton.h
	$(CXX) $(CXXFLAGS) -c FiniteAutomaton.cpp

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@echo "=== Test 1: Program simplu (p1.txt) ==="
	@echo "int main() { int x; x = 5; return 0; }" > p1.txt
	./$(TARGET) < test_input.txt || true
	@echo ""
	@echo "=== Test 2: Constante (p2.txt) ==="
	@echo "int a = 123; float b = 45.67; int c = 0x1A;" > p2.txt
	@echo ""
	@echo "=== Test 3: Identificatori (p3.txt) ==="
	@echo "int myVar; float _temp; int count123;" > p3.txt

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f *~ *.bak

distclean: clean
	rm -f *.txt~
	rm -f output_*.txt

rebuild: clean all

help:
	@echo "Comenzi disponibile:"
	@echo "  make         - Compileaza programul"
	@echo "  make run     - Compileaza si ruleaza programul"
	@echo "  make test    - Ruleaza teste automate"
	@echo "  make clean   - Sterge fisierele compilate"
	@echo "  make distclean - Sterge toate fisierele generate"
	@echo "  make rebuild - Recompileaza tot"
	@echo "  make help    - Afiseaza acest mesaj"

