yacc -dv syntax.txt
flex lex.txt
gcc -g dynstack.c symbol.c code_templates.c tree.c code_gen.c lex.yy.c y.tab.c 
