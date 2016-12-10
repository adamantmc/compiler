#Compiler
A project for the Programming Language Design class. We had to make a compiler for the following language:
```
program → declaration-list statement-list
declaration-list → declaration-list declaration | declaration
declaration → type-specifier ID ; | type-specifier ID [ NUM ] ;
type-specifier → int
statement-list → statement-list statement
| ε
statement → expression-stmt | compound-stmt | selection-stmt | iteration-stmt
| print-stmt
compound-stmt → { local-declarations statement-list }
local-declarations → local-declarations declaration
| ε
expression-stmt → expression ; | ;
selection-stmt → if ( expression ) statement
| if ( expression ) statement else statement
iteration-stmt → while ( expression ) statement
print-stmt → print expression
expression → var = expression | simple-expression
var → ID | ID [ expression ]
simple-expression → additive-expression relop additive-expression
| additive-expression
relop → <= | < | >| >=| ==| !=
additive-expression → additive-expression addop term
| term
addop → + | -
term → term mulop factor
| factor
mulop → * | /
factor → ( expression ) | var | NUM
```
#Semantics
The value of assign expressions is the value of the right hand side. <br />
The value of a simple-expression is the value of the additive-expression, or if there is a relop then it is 0 or 1 depending on the outcome of the relop's condition.

The target machine was MIX (https://en.wikipedia.org/wiki/MIX), and you will need to download the MIX Development Kit 
(https://www.gnu.org/software/mdk/) in order to run the compiler successfully. You will also need yacc and flex, for the
parser and the lexer respectively.

Be forewarned: there are quite a few points where i print debugging messages, and i was too bored to remove them.

#Compilation
You can compile with the ```make.sh``` script:
```
yacc -dv syntax.txt
flex lex.txt
gcc -g dynstack.c symbol.c code_templates.c tree.c code_gen.c lex.yy.c y.tab.c 
```
It first creates the parser, then the lexer, then creates the executable file.

#Usage
You can run the ```test.sh``` script:
```
./a.out < storeAndPrint.txt
mixasm output.mixal
mixvm -r output
```
To run a program, first you have to use the compiler to create the output.mixal file, which contains the mix assembly.
Then call the mix assembler (```mixasm```) and finally call mixvm with the machine code file for execution.

The files ```array.txt, expressions.txt, fibonacci.txt, scope.txt, storeAndPrint.txt``` contain example programs that you can run.
