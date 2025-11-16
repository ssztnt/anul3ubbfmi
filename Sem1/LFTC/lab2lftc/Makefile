# Makefile pentru Automate Finite - Lab 2 LFTC

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = fa_program
SOURCES = main.cpp FiniteAutomaton.cpp
HEADERS = FiniteAutomaton.h
OBJECTS = $(SOURCES:.cpp=.o)

# Regula principala
all: $(TARGET)

# Compilare program
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compilare fisiere obiect
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rulare program
run: $(TARGET)
	./$(TARGET)

# Test cu DFA pentru constante intregi
test_integer: $(TARGET)
	@echo "===== Test: DFA pentru Constante Intregi C/C++ ====="
	@echo "Rulare automata..."
	./$(TARGET)

# Test cu DFA simplu
test_simple: $(TARGET)
	@echo "===== Test: DFA Simplu ====="
	@echo "Rulare automata..."
	./$(TARGET)

# Curatare fisiere compilate
clean:
	rm -f $(OBJECTS) $(TARGET)

# Curatare completa (include si fisiere backup)
distclean: clean
	rm -f *~ *.bak

# Recompilare completa
rebuild: clean all

# Afisare help
help:
	@echo "Makefile pentru Automate Finite - Lab 2 LFTC"
	@echo ""
	@echo "Optiuni disponibile:"
	@echo "  make              - Compileaza programul"
	@echo "  make run          - Compileaza si ruleaza programul"
	@echo "  make test_integer - Test cu DFA pentru constante intregi"
	@echo "  make test_simple  - Test cu DFA simplu"
	@echo "  make clean        - Sterge fisierele compilate"
	@echo "  make distclean    - Sterge tot (inclusiv backup-uri)"
	@echo "  make rebuild      - Recompileaza de la zero"
	@echo "  make help         - Afiseaza acest mesaj"

.PHONY: all run test_integer test_simple clean distclean rebuild help

