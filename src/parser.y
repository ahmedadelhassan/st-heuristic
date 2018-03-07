%{

#include <stdio.h>
#include <string.h>

#include "graph.h"

void yyerror(char* str)
{
  fprintf(stderr, "error: %s\n", str);
}

/*
 * The scanner terminates (returning 0 to its caller) it receives an end-of-file
 * indication from YY_INPUT.
 */
int yywrap()
{
  return(1);
}

int main()
{
  yyparse();
  return(0);
}

%}

%token NUMBER
%token GRAPH_SECTION
%token END
%token NODES
%token EDGES
%token EDGE
%token TERMINALS_SECTION
%token TERMINALS
%token TERMINAL
%token END_OF_FILE


%code requires {
    struct node {
        char * val;
        struct node * next;
    };
}

%union {
    char * string;
    struct node args;
}
%union
{
  int    n_nodes;
  int    n_edges;
  int    n_terminals;
  graph* g;
}

%%

axiom:
  graph terminals END_OF_FILE

graph:
  GRAPH_SECTION n_nodes n_edges edges END

n_nodes:
  NODES NUMBER
  {
    printf("number of nodes %d\n", $2);
    g = graph_alloc($2);
    n_nodes = $2;
  }
  ;

n_edges:
  EDGES NUMBER
  {
    printf("number of edges %d\n", $2);
    n_edges = $2;
  }
  ;

edges:
  | edges edge

edge:
  EDGE NUMBER NUMBER NUMBER
  {
    printf("edges %d %d %d\n", $2, $3, $4);
    graph_add_edge(g, $2, $3, $4);
  }
  ;

terminals:
  TERMINALS_SECTION n_terminals terminals END

n_terminals:
  TERMINALS NUMBER
  {
    printf("number of terminals %d\n", $2);
    n_terminals = $2;
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
