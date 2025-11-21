# Makefile pentru Analizor Lexical - FLEX
# Tema 3 - LFTC

# Compilator
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Fisiere
LEX_FILE = lexer.l
LEX_OUTPUT = lex.yy.c
EXECUTABLE = lexer

# Test files
TEST_FILES = p1.txt p2.txt p3.txt p_err.txt

# Reguli
.PHONY: all clean run test help

all: $(EXECUTABLE)
	@echo "========================================="
	@echo "✓ Compilare completata cu succes!"
	@echo "========================================="

$(EXECUTABLE): $(LEX_FILE)
	@echo "========================================="
	@echo "Generare analizor lexical cu FLEX..."
	@echo "========================================="
	flex $(LEX_FILE)
	@echo ""
	@echo "Compilare cod C generat..."
	$(CC) $(CFLAGS) $(LEX_OUTPUT) -o $(EXECUTABLE)
	@echo ""

run: $(EXECUTABLE)
	@echo "========================================="
	@echo "Rulare analizor lexical - Test p1.txt"
	@echo "========================================="
	@./$(EXECUTABLE) p1.txt

test: $(EXECUTABLE)
	@echo "========================================="
	@echo "Testare analizor lexical pe toate fisierele"
	@echo "========================================="
	@for file in $(TEST_FILES); do \
		echo ""; \
		echo ">>> Testare: $$file"; \
		echo "----------------------------------------"; \
		./$(EXECUTABLE) $$file; \
		echo ""; \
	done
	@echo "========================================="
	@echo "Testare completa!"
	@echo "========================================="

clean:
	@echo "Curatare fisiere generate..."
	rm -f $(LEX_OUTPUT) $(EXECUTABLE)
	rm -f TS.txt FIP.txt
	rm -f *.o
	@echo "✓ Fisiere curatate!"

rebuild: clean all

help:
	@echo "========================================="
	@echo "   Analizor Lexical - FLEX - Help"
	@echo "========================================="
	@echo "Comenzi disponibile:"
	@echo ""
	@echo "  make          - Compileaza analizorul"
	@echo "  make all      - Compileaza analizorul"
	@echo "  make run      - Ruleaza pe p1.txt"
	@echo "  make test     - Testeaza pe toate fisierele"
	@echo "  make clean    - Sterge fisierele generate"
	@echo "  make rebuild  - Recompileaza complet"
	@echo "  make help     - Afiseaza acest mesaj"
	@echo ""
	@echo "Utilizare manuala:"
	@echo "  ./$(EXECUTABLE) <fisier_sursa>"
	@echo ""
	@echo "Exemple:"
	@echo "  ./$(EXECUTABLE) p1.txt"
	@echo "  ./$(EXECUTABLE) p2.txt"
	@echo "========================================="

