
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tabela de Simboluri (TS)
typedef struct {
    char value[256];
    int index;
} Symbol;

Symbol symbolTable[1000];
int symbolTableSize = 0;

// Forma Interna a Programului (FIP)
typedef struct {
    char code[256];
    int position;
} FIPEntry;

FIPEntry fip[10000];
int fipSize = 0;

// Erori
typedef struct {
    char message[512];
    int line;
    int column;
} Error;

Error errors[1000];
int errorCount = 0;

// Contoare
int lineNumber = 1;
int columnNumber = 1;
int errorOccurred = 0;

// Functii helper
int addToSymbolTable(const char* value);
void addToFIP(const char* code, int position);
void addError(const char* message, int line, int col);
void displaySymbolTable();
void displayFIP();
void displayErrors();
void displayAll();
void saveToFile(const char* filename, int type);

%}

/* Optiuni FLEX */
%option noyywrap
%option yylineno

/* Definitii regulate */
DIGIT           [0-9]
LETTER          [a-zA-Z]
UNDERSCORE      "_"

/* Identificatori: [a-zA-Z_][a-zA-Z0-9_]* */
IDENTIFIER      ({LETTER}|{UNDERSCORE})({LETTER}|{DIGIT}|{UNDERSCORE})*

/* Constante intregi */
DECIMAL         [+-]?([1-9]{DIGIT}*|0)
OCTAL           0[0-7]+
HEXADECIMAL     0[xX][0-9a-fA-F]+
INTEGER         {DECIMAL}|{OCTAL}|{HEXADECIMAL}

/* Constante reale */
REAL_SIMPLE     [+-]?{DIGIT}+\.{DIGIT}*
REAL_DOT_START  [+-]?\.{DIGIT}+
EXPONENT        [eE][+-]?{DIGIT}+
REAL            ({REAL_SIMPLE}|{REAL_DOT_START}){EXPONENT}?

/* Whitespace */
WHITESPACE      [ \t\r]+
NEWLINE         \n

/* Comentarii (optional) */
COMMENT_LINE    "//".*
COMMENT_BLOCK   "/*"([^*]|\*+[^*/])*\*+"/"

%%

    /* Cuvinte cheie */
"int"           { addToFIP("int", 0); columnNumber += yyleng; }
"float"         { addToFIP("float", 0); columnNumber += yyleng; }
"char"          { addToFIP("char", 0); columnNumber += yyleng; }
"string"        { addToFIP("string", 0); columnNumber += yyleng; }
"bool"          { addToFIP("bool", 0); columnNumber += yyleng; }
"if"            { addToFIP("if", 0); columnNumber += yyleng; }
"else"          { addToFIP("else", 0); columnNumber += yyleng; }
"while"         { addToFIP("while", 0); columnNumber += yyleng; }
"for"           { addToFIP("for", 0); columnNumber += yyleng; }
"do"            { addToFIP("do", 0); columnNumber += yyleng; }
"return"        { addToFIP("return", 0); columnNumber += yyleng; }
"void"          { addToFIP("void", 0); columnNumber += yyleng; }
"main"          { addToFIP("main", 0); columnNumber += yyleng; }
"read"          { addToFIP("read", 0); columnNumber += yyleng; }
"write"         { addToFIP("write", 0); columnNumber += yyleng; }
"true"          { addToFIP("true", 0); columnNumber += yyleng; }
"false"         { addToFIP("false", 0); columnNumber += yyleng; }

    
"cattimp"       { addToFIP("cattimp", 0); columnNumber += yyleng; }
"executa"       { addToFIP("executa", 0); columnNumber += yyleng; }
"sfcattimp"     { addToFIP("sfcattimp", 0); columnNumber += yyleng; }

    
"++"            { addToFIP("++", 0); columnNumber += yyleng; }
"--"            { addToFIP("--", 0); columnNumber += yyleng; }
"+="            { addToFIP("+=", 0); columnNumber += yyleng; }
"-="            { addToFIP("-=", 0); columnNumber += yyleng; }
"*="            { addToFIP("*=", 0); columnNumber += yyleng; }
"/="            { addToFIP("/=", 0); columnNumber += yyleng; }
"=="            { addToFIP("==", 0); columnNumber += yyleng; }
"!="            { addToFIP("!=", 0); columnNumber += yyleng; }
"<="            { addToFIP("<=", 0); columnNumber += yyleng; }
">="            { addToFIP(">=", 0); columnNumber += yyleng; }
"&&"            { addToFIP("&&", 0); columnNumber += yyleng; }
"||"            { addToFIP("||", 0); columnNumber += yyleng; }

    /* Operatori simpli */
"+"             { addToFIP("+", 0); columnNumber += yyleng; }
"-"             { addToFIP("-", 0); columnNumber += yyleng; }
"*"             { addToFIP("*", 0); columnNumber += yyleng; }
"/"             { addToFIP("/", 0); columnNumber += yyleng; }
"%"             { addToFIP("%", 0); columnNumber += yyleng; }
"="             { addToFIP("=", 0); columnNumber += yyleng; }
"<"             { addToFIP("<", 0); columnNumber += yyleng; }
">"             { addToFIP(">", 0); columnNumber += yyleng; }
"!"             { addToFIP("!", 0); columnNumber += yyleng; }

    /* Delimitatori */
"("             { addToFIP("(", 0); columnNumber += yyleng; }
")"             { addToFIP(")", 0); columnNumber += yyleng; }
"{"             { addToFIP("{", 0); columnNumber += yyleng; }
"}"             { addToFIP("}", 0); columnNumber += yyleng; }
"["             { addToFIP("[", 0); columnNumber += yyleng; }
"]"             { addToFIP("]", 0); columnNumber += yyleng; }
";"             { addToFIP(";", 0); columnNumber += yyleng; }
","             { addToFIP(",", 0); columnNumber += yyleng; }
"."             { addToFIP(".", 0); columnNumber += yyleng; }
":"             { addToFIP(":", 0); columnNumber += yyleng; }

    /* Constante reale (trebuie inainte de intregi) */
{REAL}          { 
                    int pos = addToSymbolTable(yytext);
                    addToFIP("CONST_REAL", pos);
                    columnNumber += yyleng;
                }

    /* Constante intregi */
{INTEGER}       { 
                    int pos = addToSymbolTable(yytext);
                    addToFIP("CONST_INT", pos);
                    columnNumber += yyleng;
                }

    /* Identificatori */
{IDENTIFIER}    { 
                    int pos = addToSymbolTable(yytext);
                    addToFIP("ID", pos);
                    columnNumber += yyleng;
                }

    /* Comentarii (ignorate) */
{COMMENT_LINE}  { columnNumber += yyleng; }
{COMMENT_BLOCK} { 
                    for (int i = 0; i < yyleng; i++) {
                        if (yytext[i] == '\n') {
                            lineNumber++;
                            columnNumber = 1;
                        } else {
                            columnNumber++;
                        }
                    }
                }

    /* Whitespace */
{WHITESPACE}    { columnNumber += yyleng; }
{NEWLINE}       { lineNumber++; columnNumber = 1; }

    /* Eroare - token nerecunoscut */
.               { 
                    char msg[512];
                    snprintf(msg, sizeof(msg), "Caracter invalid: '%c' (ASCII %d)", 
                             yytext[0], (int)yytext[0]);
                    addError(msg, lineNumber, columnNumber);
                    columnNumber += yyleng;
                    errorOccurred = 1;
                }

%%

/* Implementare functii */

int addToSymbolTable(const char* value) {
    // Cauta daca exista deja in TS
    for (int i = 0; i < symbolTableSize; i++) {
        if (strcmp(symbolTable[i].value, value) == 0) {
            return symbolTable[i].index;
        }
    }
    
    // Adauga nou simbol
    strcpy(symbolTable[symbolTableSize].value, value);
    symbolTable[symbolTableSize].index = symbolTableSize;
    symbolTableSize++;
    return symbolTableSize - 1;
}

void addToFIP(const char* code, int position) {
    strcpy(fip[fipSize].code, code);
    fip[fipSize].position = position;
    fipSize++;
}

void addError(const char* message, int line, int col) {
    snprintf(errors[errorCount].message, sizeof(errors[errorCount].message), 
             "Linia %d, Coloana %d: %s", line, col, message);
    errors[errorCount].line = line;
    errors[errorCount].column = col;
    errorCount++;
}

void displaySymbolTable() {
    printf("\n========================================\n");
    printf("       TABELA DE SIMBOLURI (TS)        \n");
    printf("========================================\n");
    printf("%-8s %-30s\n", "Index", "Simbol");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < symbolTableSize; i++) {
        printf("%-8d %-30s\n", symbolTable[i].index, symbolTable[i].value);
    }
    
    printf("----------------------------------------\n");
    printf("Total simboluri: %d\n", symbolTableSize);
    printf("========================================\n\n");
}

void displayFIP() {
    printf("\n========================================\n");
    printf("  FORMA INTERNA A PROGRAMULUI (FIP)   \n");
    printf("========================================\n");
    printf("%-8s %-20s %-10s\n", "Nr.", "Cod Atom", "Poz TS");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < fipSize; i++) {
        printf("%-8d %-20s %-10d\n", i, fip[i].code, fip[i].position);
    }
    
    printf("----------------------------------------\n");
    printf("Total atomi: %d\n", fipSize);
    printf("========================================\n\n");
}

void displayErrors() {
    printf("\n========================================\n");
    printf("          ERORI LEXICALE               \n");
    printf("========================================\n");
    
    if (errorCount == 0) {
        printf("✓ Nu s-au gasit erori lexicale!\n");
    } else {
        printf("✗ S-au gasit %d erori:\n\n", errorCount);
        for (int i = 0; i < errorCount; i++) {
            printf("%d. %s\n", i + 1, errors[i].message);
        }
    }
    
    printf("========================================\n\n");
}

void displayAll() {
    displaySymbolTable();
    displayFIP();
    displayErrors();
}

void saveToFile(const char* filename, int type) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Eroare: Nu se poate deschide fisierul %s\n", filename);
        return;
    }
    
    if (type == 1) { // TS
        fprintf(file, "========================================\n");
        fprintf(file, "       TABELA DE SIMBOLURI (TS)        \n");
        fprintf(file, "========================================\n");
        fprintf(file, "%-8s %-30s\n", "Index", "Simbol");
        fprintf(file, "----------------------------------------\n");
        
        for (int i = 0; i < symbolTableSize; i++) {
            fprintf(file, "%-8d %-30s\n", symbolTable[i].index, symbolTable[i].value);
        }
        
        fprintf(file, "----------------------------------------\n");
        fprintf(file, "Total simboluri: %d\n", symbolTableSize);
    } else if (type == 2) { // FIP
        fprintf(file, "========================================\n");
        fprintf(file, "  FORMA INTERNA A PROGRAMULUI (FIP)   \n");
        fprintf(file, "========================================\n");
        fprintf(file, "%-8s %-20s %-10s\n", "Nr.", "Cod Atom", "Poz TS");
        fprintf(file, "----------------------------------------\n");
        
        for (int i = 0; i < fipSize; i++) {
            fprintf(file, "%-8d %-20s %-10d\n", i, fip[i].code, fip[i].position);
        }
        
        fprintf(file, "----------------------------------------\n");
        fprintf(file, "Total atomi: %d\n", fipSize);
    }
    
    fclose(file);
    printf("✓ Salvat in %s\n", filename);
}

int main(int argc, char** argv) {
    printf("========================================\n");
    printf("   ANALIZOR LEXICAL - LEX/FLEX         \n");
    printf("   LFTC - Tema 3                       \n");
    printf("========================================\n\n");
    
    FILE* input = stdin;
    
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Eroare: Nu se poate deschide fisierul %s\n", argv[1]);
            return 1;
        }
        printf("Analizare fisier: %s\n\n", argv[1]);
        yyin = input;
    } else {
        printf("Utilizare: %s <fisier_sursa>\n", argv[0]);
        printf("sau citire de la stdin (Ctrl+D pentru a termina)\n\n");
    }
    
    // Ruleaza analizorul lexical
    yylex();
    
    if (input != stdin) {
        fclose(input);
    }
    
    // Afiseaza rezultatele
    displayAll();
    
    // Salveaza in fisiere
    if (argc > 1) {
        saveToFile("TS.txt", 1);
        saveToFile("FIP.txt", 2);
    }
    
    // Exit code
    if (errorOccurred) {
        printf("\n✗ Analiza completata cu ERORI!\n\n");
        return 1;
    } else {
        printf("\n✓ Analiza completata cu SUCCES!\n\n");
        return 0;
    }
}

