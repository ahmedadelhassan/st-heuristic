%{
#include <stdio.h>
#include "parser.tab.h"
%}

%%
"SECTION Graph"     return GRAPH_SECTION;
"SECTION Terminals" return TERMINALS_SECTION;
"END"               return END;
"EOF"               return END_OF_FILE;
"Nodes"             return NODES;
"Edges"             return EDGES;
"Terminals"         return TERMINALS;
"E"                 return EDGE;
"T"                 return TERMINAL;
[0123456789]+       yylval = atoi(yytext); return NUMBER;
\n                  /* ignore end of line */;
[ \t]+              /* ignore whitespace  */;
%%
