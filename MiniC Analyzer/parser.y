%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int   yylineno;
extern char *yytext;
extern FILE *yyin;

int  yylex(void);
void yyerror(const char *s);

/* --------- error counters --------- */
static int syntax_errors = 0;
static int sem_errors    = 0;

/* ================================================================
 * Symbol table
 * ================================================================ */
#define MAX_SYM 256
#define SLEN     64

typedef struct {
    char name [SLEN];
    char class[16];   /* variable | funcion | macro | parametro */
    char type [16];   /* int | float | char | double | void      */
    int  arity;
    int  scope;
    int  used;
    int  active;
} Sym;

static Sym sym[MAX_SYM];
static int sym_n     = 0;
static int cur_scope = 0;

/* Current declaration context */
static char cur_type     [16]   = "int";
static char cur_func     [SLEN] = "";
static char cur_func_type[16]   = "int";
static int  cur_arity           = 0;

/* ---------- scope helpers ---------- */
static void scope_push(void) { cur_scope++; }
static void scope_pop(void) {
    for (int i = 0; i < sym_n; i++)
        if (sym[i].scope == cur_scope) sym[i].active = 0;
    cur_scope--;
}

/* ---------- lookup helpers ---------- */
static int sym_active(const char *n) {
    for (int i = sym_n - 1; i >= 0; i--)
        if (sym[i].active && strcmp(sym[i].name, n) == 0)
            return i;
    return -1;
}
static int sym_local(const char *n) {
    for (int i = 0; i < sym_n; i++)
        if (sym[i].active && sym[i].scope == cur_scope &&
            strcmp(sym[i].name, n) == 0)
            return i;
    return -1;
}
static int sym_global(const char *n) {
    for (int i = 0; i < sym_n; i++)
        if (sym[i].scope == 0 && strcmp(sym[i].name, n) == 0)
            return i;
    return -1;
}
static void sym_add(const char *n, const char *cl, const char *ty, int ar) {
    if (sym_n >= MAX_SYM) return;
    strncpy(sym[sym_n].name,  n,  SLEN - 1);
    strncpy(sym[sym_n].class, cl, 15);
    strncpy(sym[sym_n].type,  ty, 15);
    sym[sym_n].arity  = ar;
    sym[sym_n].scope  = cur_scope;
    sym[sym_n].used   = 0;
    sym[sym_n].active = 1;
    sym_n++;
}

/* ---------- semantic helpers ---------- */
static void check_var(const char *n) {
    int idx = sym_active(n);
    if (idx < 0) {
        fprintf(stderr,
            "Error semántico en línea %d: variable '%s' no declarada\n",
            yylineno, n);
        sem_errors++;
    } else {
        sym[idx].used = 1;
    }
}

static void check_func(const char *n, int argc) {
    int idx = sym_global(n);
    if (idx < 0) {
        fprintf(stderr,
            "Error semántico en línea %d: función '%s' no declarada\n",
            yylineno, n);
        sem_errors++;
    } else if (strcmp(sym[idx].class, "funcion") == 0 &&
               sym[idx].arity != argc) {
        fprintf(stderr,
            "Error semántico en línea %d: función '%s' espera %d"
            " argumento(s), pero recibió %d\n",
            yylineno, n, sym[idx].arity, argc);
        sem_errors++;
    }
}

/* ---------- print symbol table ---------- */
static void sym_print(void) {
    const char *sep =
        "+--------------------+------------+--------+--------+--------+\n";
    printf("\n%s", sep);
    printf("| %-18s | %-10s | %-6s | %-6s | %-6s |\n",
           "Nombre", "Clase", "Tipo", "Ámbito", "Aridad");
    printf("%s", sep);
    for (int i = 0; i < sym_n; i++) {
        char ar_buf[8] = "";
        if (strcmp(sym[i].class, "funcion") == 0)
            snprintf(ar_buf, sizeof(ar_buf), "%d", sym[i].arity);
        printf("| %-18s | %-10s | %-6s | %-6d | %-6s |\n",
               sym[i].name, sym[i].class, sym[i].type,
               sym[i].scope, ar_buf);
    }
    printf("%s\n", sep);
}

/* ---------- check for unused local variables ---------- */
static void check_unused(void) {
    for (int i = 0; i < sym_n; i++) {
        if (strcmp(sym[i].class, "variable") == 0 &&
            sym[i].scope > 0 &&
            !sym[i].used) {
            fprintf(stderr,
                "Advertencia: variable '%s' declarada pero no usada\n",
                sym[i].name);
        }
    }
}

/* ---------- syntax error helper ---------- */
static void report_context_error(const char *ctx) {
    syntax_errors++;
    printf("\n[ERROR SINTÁCTICO]\n");
    printf("  Línea aproximada : %d\n", yylineno);
    printf("  Cerca de lexema  : '%s'\n", yytext);
    printf("  Contexto         : %s\n\n", ctx);
}
%}

%define parse.error verbose

%union {
    char *sval;
    int   ival;
}

%token          INCLUDE DEFINE
%token          INT FLOAT CHAR DOUBLE VOID
%token          RETURN IF ELSE WHILE FUNC
%token <sval>   ID
%token          NUMBER FLOAT_NUMBER STRING_LITERAL
%token          LE GE EQ NE

%type  <ival>   argument_list argument_list_opt

%left  '+' '-'
%left  '*' '/'
%left  '<' '>' LE GE EQ NE
%right '='

%%

/* ================================================================
 * Top-level program
 * ================================================================ */
program:
    includes macros global_declarations functions
    {
        check_unused();
        sym_print();
        if (syntax_errors == 0 && sem_errors == 0) {
            printf("Análisis completado correctamente.\n");
        } else {
            printf("Análisis terminado con %d error(es) sintáctico(s) y"
                   " %d error(es) semántico(s).\n",
                   syntax_errors, sem_errors);
        }
    }
  ;

/* ================================================================
 * Includes
 * ================================================================ */
includes:
    includes include_stmt
  | /* empty */
  ;

include_stmt:
    INCLUDE '<' ID '>'           { free($3); }
  | INCLUDE '<' ID '.' ID '>'   { free($3); free($5); }
  | INCLUDE STRING_LITERAL
  | INCLUDE error
    { report_context_error("Directiva #include inválida."); yyerrok; }
  ;

/* ================================================================
 * Macros
 * ================================================================ */
macros:
    macros macro_stmt
  | /* empty */
  ;

macro_stmt:
    DEFINE ID expression
    {
        if (sym_global($2) >= 0) {
            fprintf(stderr,
                "Error semántico en línea %d: macro '%s' ya definida\n",
                yylineno, $2);
            sem_errors++;
        } else {
            sym_add($2, "macro", "int", 0);
        }
        free($2);
    }
  | DEFINE ID
    {
        if (sym_global($2) >= 0) {
            fprintf(stderr,
                "Error semántico en línea %d: macro '%s' ya definida\n",
                yylineno, $2);
            sem_errors++;
        } else {
            sym_add($2, "macro", "int", 0);
        }
        free($2);
    }
  | DEFINE error
    { report_context_error("Declaración de macro inválida."); yyerrok; }
  ;

/* ================================================================
 * Global variable declarations
 * ================================================================ */
global_declarations:
    global_declarations declaration
  | /* empty */
  ;

declaration:
    type declarator_list ';'
  | type error ';'
    { report_context_error("Declaración de variable inválida."); yyerrok; }
  ;

declarator_list:
    declarator
  | declarator_list ',' declarator
  ;

declarator:
    ID
    {
        int dup = (cur_scope == 0) ? sym_global($1) >= 0
                                   : sym_local($1) >= 0;
        if (dup) {
            fprintf(stderr,
                "Error semántico en línea %d: redeclaración de variable '%s'\n",
                yylineno, $1);
            sem_errors++;
        } else {
            sym_add($1, "variable", cur_type, 0);
        }
        free($1);
    }
  | ID '=' expression
    {
        int dup = (cur_scope == 0) ? sym_global($1) >= 0
                                   : sym_local($1) >= 0;
        if (dup) {
            fprintf(stderr,
                "Error semántico en línea %d: redeclaración de variable '%s'\n",
                yylineno, $1);
            sem_errors++;
        } else {
            sym_add($1, "variable", cur_type, 0);
        }
        free($1);
    }
  ;

type:
    INT    { strncpy(cur_type, "int",    15); }
  | FLOAT  { strncpy(cur_type, "float",  15); }
  | CHAR   { strncpy(cur_type, "char",   15); }
  | DOUBLE { strncpy(cur_type, "double", 15); }
  | VOID   { strncpy(cur_type, "void",   15); }
  ;

/* ================================================================
 * Functions
 * ================================================================ */
functions:
    any_function
  | functions any_function
  ;

any_function:
    function
  | func_definition
  ;

/* function_head avoids mid-rule action conflicts */
function_head:
    type ID
    {
        strncpy(cur_func,      $2,       SLEN - 1);
        strncpy(cur_func_type, cur_type, 15);
        cur_arity = 0;
        if (sym_global($2) >= 0) {
            fprintf(stderr,
                "Error semántico en línea %d: función '%s' ya declarada\n",
                yylineno, $2);
            sem_errors++;
        }
        scope_push();
        free($2);
    }
  ;

/* C-style:  int suma(int a, int b) { ... } */
function:
    function_head '(' parameter_list_opt ')' block
    {
        if (sym_global(cur_func) < 0) {
            int s = cur_scope; cur_scope = 0;
            sym_add(cur_func, "funcion", cur_func_type, cur_arity);
            cur_scope = s;
        }
        scope_pop();
    }
  | function_head '(' error ')' block
    {
        report_context_error("Error en parámetros de función.");
        scope_pop();
        yyerrok;
    }
  | function_head '(' parameter_list_opt ')' error
    {
        report_context_error("Bloque de función inválido.");
        scope_pop();
        yyerrok;
    }
  ;

parameter_list_opt:
    parameter_list
  | /* empty */
  ;

parameter_list:
    parameter
  | parameter_list ',' parameter
  ;

parameter:
    type ID
    {
        sym_add($2, "parametro", cur_type, 0);
        cur_arity++;
        free($2);
    }
  | type
    { cur_arity++; }
  ;

/* func_head avoids mid-rule action conflicts */
func_head:
    FUNC ID
    {
        strncpy(cur_func,      $2,    SLEN - 1);
        strncpy(cur_func_type, "int", 15);
        cur_arity = 0;
        if (sym_global($2) >= 0) {
            fprintf(stderr,
                "Error semántico en línea %d: función '%s' ya declarada\n",
                yylineno, $2);
            sem_errors++;
        }
        scope_push();
        free($2);
    }
  ;

/* func-style:  func suma(a, b) { ... } */
func_definition:
    func_head '(' id_list_opt ')' block
    {
        if (sym_global(cur_func) < 0) {
            int s = cur_scope; cur_scope = 0;
            sym_add(cur_func, "funcion", cur_func_type, cur_arity);
            cur_scope = s;
        }
        scope_pop();
    }
  ;

id_list_opt:
    id_list
  | /* empty */
  ;

id_list:
    ID
    {
        sym_add($1, "parametro", "int", 0);
        cur_arity++;
        free($1);
    }
  | id_list ',' ID
    {
        sym_add($3, "parametro", "int", 0);
        cur_arity++;
        free($3);
    }
  ;

/* ================================================================
 * Block — block_open avoids duplicate mid-rule action conflicts
 * ================================================================ */
block_open:
    '{'
    { scope_push(); }
  ;

block:
    block_open local_declarations statements '}'
    { scope_pop(); }
  | block_open local_declarations statements error
    { scope_pop(); report_context_error("Falta cerrar bloque '}'."); yyerrok; }
  ;

local_declarations:
    local_declarations declaration
  | /* empty */
  ;

/* ================================================================
 * Statements
 * ================================================================ */
statements:
    statements statement
  | /* empty */
  ;

statement:
    assignment_stmt
  | if_stmt
  | while_stmt
  | return_stmt
  | function_call_stmt
  | block
  | error ';'
    { report_context_error("Sentencia inválida."); yyerrok; }
  ;

assignment_stmt:
    ID '=' expression ';'
    {
        int idx = sym_active($1);
        if (idx < 0) {
            fprintf(stderr,
                "Error semántico en línea %d: variable '%s' no declarada\n",
                yylineno, $1);
            sem_errors++;
        } else {
            sym[idx].used = 1;  /* count LHS write as "used" */
        }
        free($1);
    }
  | ID '=' error ';'
    {
        report_context_error("Asignación inválida.");
        free($1);
        yyerrok;
    }
  ;

function_call_stmt:
    ID '(' argument_list_opt ')' ';'
    {
        check_func($1, $3);
        free($1);
    }
  | ID '(' error ')' ';'
    {
        report_context_error("Llamada a función inválida.");
        free($1);
        yyerrok;
    }
  ;

argument_list_opt:
    argument_list  { $$ = $1; }
  | /* empty */    { $$ = 0; }
  ;

argument_list:
    expression                    { $$ = 1; }
  | argument_list ',' expression  { $$ = $1 + 1; }
  ;

if_stmt:
    IF '(' expression ')' block
  | IF '(' expression ')' block ELSE block
  | IF '(' error ')' block
    { report_context_error("Condición inválida en if."); yyerrok; }
  | IF '(' expression ')' error
    { report_context_error("Bloque inválido en if."); yyerrok; }
  ;

while_stmt:
    WHILE '(' expression ')' block
  | WHILE '(' error ')' block
    { report_context_error("Condición inválida en while."); yyerrok; }
  ;

return_stmt:
    RETURN expression ';'
  | RETURN ';'
  | RETURN error ';'
    { report_context_error("Sentencia return inválida."); yyerrok; }
  ;

/* ================================================================
 * Expressions  (side-effects: check_var / check_func)
 * ================================================================ */
expression:
    expression '+' expression
  | expression '-' expression
  | expression '*' expression
  | expression '/' expression
  | expression '<' expression
  | expression '>' expression
  | expression LE  expression
  | expression GE  expression
  | expression EQ  expression
  | expression NE  expression
  | '(' expression ')'
  | ID '(' argument_list_opt ')'
    {
        check_func($1, $3);
        free($1);
    }
  | ID
    {
        check_var($1);
        free($1);
    }
  | NUMBER
  | FLOAT_NUMBER
  | STRING_LITERAL
  ;

%%

void yyerror(const char *s) {
    syntax_errors++;
    printf("\n[ERROR SINTÁCTICO GENERAL]\n");
    printf("  Línea aproximada : %d\n", yylineno);
    printf("  Cerca de lexema  : '%s'\n", yytext);
    printf("  Mensaje Bison    : %s\n\n", s);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "Error: no se puede abrir '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
        yyin = f;
    }
    printf("Iniciando análisis...\n\n");
    yyparse();
    if (argc > 1) fclose(yyin);
    return (syntax_errors + sem_errors == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
