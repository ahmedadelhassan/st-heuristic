%{

#include <stdio.h>
#include <string.h>

/* #define YYSTYPE int */

void yyerror(char* str)
{
  fprintf(stderr, "error: %s\n", str);
}

int yywrap()
{
  return 1;
}

int main()
{
  yyparse();
  return(0);
}

%}

%token NUMBER GRAPH_SECTION END NODES EDGES EDGE TERMINALS_SECTION TERMINALS TERMINAL END_OF_FILE

%%

axiom:
  graph terminals END_OF_FILE

graph:
  GRAPH_SECTION n_nodes n_edges edges END

n_nodes:
  NODES NUMBER
  {
    printf("number of nodes %d\n", $2);
  }
  ;

n_edges:
  EDGES NUMBER
  {
    printf("number of edges %d\n", $2);
  }
  ;

edges:
  | edges edge

edge:
  EDGE NUMBER NUMBER NUMBER
  {
    printf("edges %d %d %d\n", $2, $3, $4);
  }
  ;

terminals:
  TERMINALS_SECTION n_terminals terminals END

n_terminals:
  TERMINALS NUMBER
  {
    printf("number of terminals %d\n", $2);
  }
  ;

terminals:
  | terminals terminal

terminal:
  TERMINAL NUMBER
  {
    printf("terminal %d\n", $2);
  }
  ;
