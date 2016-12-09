#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_gen.h"
#include "code_templates.h"
#include "symbol.h"
#include "dynstack.h"

#define DEBUG 0

extern int scope, curr_addr, stack_top_addr = 3900;
extern FILE *file;

int label_counter = 0;

void codeGeneration(Node *node) {
    if(DEBUG) printf("CODE_GENERATION: %s\n",getTypeName(node->nodeType));
    switch(node->nodeType) {
    case expStatementNode:
        codegen_expressionStatementNode(node);
        break;
    case expStatementSemicolonNode:
        break;
    case compStatementNode:
        codegen_compoundStatementNode(node);
        break;
    case selStatementNode:
        codegen_selectionStatementNode(node);
        break;
    case iterStatementNode:
        codegen_iterationStatementNode(node);
        break;
    case printStatementNode:
        codegen_printStatementNode(node);
        break;
    case localDeclarationsNode:
        codegen_localDeclarationsNode(node);
        break;
    case expAssignNode:
        codegen_expressionAssignNode(node);
        break;
    case simpleExpRelopNode:
        codegen_simpleExpressionNode(node);
        break;
    case simpleExpNode:
        codegen_simpleExpressionNode(node);
        break;
    case varNode:
        codegen_varNode(node);
        break;
    case varExpressionNode:
        codegen_varNode(node);
        break;
    case addExpressionNode:
        codegen_additiveExpressionNode(node);
        break;
    case termNode:
        codegen_termNode(node);
        break;
    case factorExpressionNode:
        codegen_factorNode(node);
        break;
    case intLiteralNode:
        codegen_intLiteralNode(node);
        break;
    case declNode:
        codegen_declarationNode(node);
        break;
    case declListNode:
        codegen_declListNode(node);
        break;
    case statListNode:
        codegen_statListNode(node);
        break;
    case programNode:
        codegen_programNode(node);
        break;
    default:
        break;
    }
}

void codegen_programNode(Node *node) {
    codeGeneration(node->n1);
    codeGeneration(node->n2);
}

void codegen_declListNode(Node *node) {
    codeGeneration(node->n1);
    codeGeneration(node->n2);
}

void codegen_localDeclarationsNode(Node *node) {
    codeGeneration(node->n1);
    codeGeneration(node->n2);
}

void codegen_declarationNode(Node *node) {
    if(node->number == -1) {
        printf("%s %s;\n",node->n1->identifier, node->identifier);
    }
    else {
        printf("%s %s[%d];\n",node->n1->identifier, node->identifier, node->number);
    }
}

void codegen_statListNode(Node *node) {
    codeGeneration(node->n1);
    codeGeneration(node->n2);
}

void codegen_expressionStatementNode(Node *node) {
    codeGeneration(node->n1);
}

void codegen_compoundStatementNode(Node *node) {

    scope++;
    scope_stack_push(scope);
    codeGeneration(node->n1);
    codeGeneration(node->n2);
    scope = scope_stack_pop();
}

void codegen_selectionStatementNode(Node *node) {
    int label = label_counter;
    label_counter++;

    codeGeneration(node->n1);
    Symbol *if_symbol = pop_symbol();

    //If the condition is not satisfied, jump to JLABi if there is an else, or jump to end.
    //If the condition is satisfied execute some code (node->n2) and jump to JENDi.
    switch(if_symbol->exp_type) {
        case EXP_L:
        if(node->n3 != NULL) fprintf(file, "\t\tJGE\tJLAB%d\n", label);
        else fprintf(file, "\t\tJGE\tJEND%d\n", label);
        break;
        case EXP_LEQ:
        if(node->n3 != NULL) fprintf(file, "\t\tJG\tJLAB%d\n", label);
        else fprintf(file, "\t\tJG\tJEND%d\n", label);
        break;
        case EXP_G:
        if(node->n3 != NULL) fprintf(file, "\t\tJLE\tJLAB%d\n", label);
        else fprintf(file, "\t\tJLE\tJEND%d\n", label);
        break;
        case EXP_GEQ:
        if(node->n3 != NULL) fprintf(file, "\t\tJL \tJLAB%d\n", label);
        else fprintf(file, "\t\tJL \tJEND%d\n", label);
        break;
        case EXP_EQ:
        if(node->n3 != NULL) fprintf(file, "\t\tJNE\tJLAB%d\n", label);
        else fprintf(file, "\t\tJNE\tJEND%d\n", label);
        break;
        case EXP_NEQ:
        if(node->n3 != NULL) fprintf(file, "\t\tJE \tJLAB%d\n", label);
        else fprintf(file, "\t\tJE \tJEND%d\n", label);
        break;
    }

    //Generate code for if statement, and then jump to end
    codeGeneration(node->n2);
    fprintf(file, "\t\tJMP\tJEND%d\n", label);

    //If there is an else, we must make a label for it, and generate its code
    if(node->n3 != NULL) {
        //Set a label before the else statement, so we can jump if needed
        fprintf(file, "JLAB%d\t\tNOP\n", label);
        codeGeneration(node->n3);
    }

    fprintf(file, "JEND%d\t\tNOP\n", label);
}

void codegen_iterationStatementNode(Node *node) {
    int label = label_counter;
    label_counter++;

    //Jump - While Start i (JWSTi)
    fprintf(file, "JWST%d\t\tNOP\n", label);

    codeGeneration(node->n1);
    Symbol *while_symbol = pop_symbol();

    //Jump to JWENi if the condition is not satisfied
    switch(while_symbol->exp_type) {
        case EXP_L:
        fprintf(file, "\t\tJGE\tJWEN%d\n", label);
        break;
        case EXP_LEQ:
        fprintf(file, "\t\tJG\tJWEN%d\n", label);
        break;
        case EXP_G:
        fprintf(file, "\t\tJLE\tJWEN%d\n", label);
        break;
        case EXP_GEQ:
        fprintf(file, "\t\tJL \tJWEN%d\n", label);
        break;
        case EXP_EQ:
        fprintf(file, "\t\tJNE\tJWEN%d\n", label);
        break;
        case EXP_NEQ:
        fprintf(file, "\t\tJE \tJWEN%d\n", label);
        break;
    }

    //The code inside the loop
    codeGeneration(node->n2);
    //Jump back to the condition checking
    fprintf(file, "\t\tJMP\tJWST%d\n",label);

    //Jump - While End i (JWENi)
    fprintf(file, "JWEN%d\t\tNOP\n", label);
}

void codegen_printStatementNode(Node *node) {
    codeGeneration(node->n1);
    Symbol *symbol = pop_symbol();
    emitPrintCode(symbol);
}

void codegen_expressionAssignNode(Node *node) {
    codeGeneration(node->n1);
    Symbol *variable = pop_symbol();
    printf("Assign: %s\n", variable->id);
    codeGeneration(node->n2);
    Symbol *value = pop_symbol();

    //Load value to rA
    if(value->type == INTEGER_TYPE) {
        fprintf(file, "\t\tLDA\t%d Load variable in rA\n",value->address);
    }
    else if(value->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tLDA\t%d,1\n", value->address, stack_top_addr);
    }
    else if(value->type == INT_CONSTANT) {
        fprintf(file, "\t\tLDA\t=%d= Enter number in rA\n", value->value);
    }

    //Store rA in variable
    if(variable->type == INTEGER_TYPE) {
        fprintf(file, "\t\tSTA\t%d\n", variable->address);
    }
    else if(variable->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tENDLABEL\n");
        stack_top_addr--;
        fprintf(file, "\t\tSTA\t%d,1\n", variable->address);
    }

    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));

    sym->storedIn = STORED_IN_RA;
    sym->type = EXPRESSION;
    push_symbol(sym);
}

void codegen_simpleExpressionNode(Node *node) {
    codeGeneration(node->n3);
    Symbol *rhs = pop_symbol();

    //Store rhs
    if(rhs->type == EXPRESSION) {
        stack_top_addr++;
        fprintf(file, "\t\tSTA\t%d\n", stack_top_addr);
    }

    codeGeneration(node->n1);
    Symbol *lhs = pop_symbol();

    emitCmpCode(lhs, rhs);

    Symbol *symbol = (Symbol *)malloc(sizeof(Symbol));

    int label = label_counter;
    label_counter++;

    //If the condition is not satisfied, jump to CJi.
    switch(node->n2->nodeType) {
    case leNode:
        symbol->exp_type = EXP_L;
        fprintf(file, "\t\tJGE\tCJ%d\n", label);
        break;
    case leEqNode:
        symbol->exp_type = EXP_LEQ;
        fprintf(file, "\t\tJG\tCJ%d\n", label);
        break;
    case grNode:
        symbol->exp_type = EXP_G;
        fprintf(file, "\t\tJLE\tCJ%d\n", label);
        break;
    case grEqNode:
        symbol->exp_type = EXP_GEQ;
        fprintf(file, "\t\tJL\tCJ%d\n", label);
        break;
    case eqNode:
        symbol->exp_type = EXP_EQ;
        fprintf(file, "\t\tJNE\tCJ%d\n", label);
        break;
    case nEqNode:
        symbol->exp_type = EXP_NEQ;
        fprintf(file, "\t\tJE\tCJ%d\n", label);
        break;
    }

    //The result of the expression is either 1 or 0

    fprintf(file, "\t\tLDA\t=1=\n");
    fprintf(file, "\t\tJMP\tCE%d\n", label);

    fprintf(file, "CJ%d\t\tNOP\n", label);
    fprintf(file, "\t\tLDA\t=0=\n");

    fprintf(file, "CE%d\t\tNOP\n", label);

    symbol->type = EXPRESSION;

    push_symbol(symbol);
}

void codegen_additiveExpressionNode(Node *node) {
    //Get rhs value, if it is an expression push it in stack
    //Then get the value of the lhs and add or substract one another.

    codeGeneration(node->n3);
    Symbol *rhs = pop_symbol();

    if(rhs->type == EXPRESSION) {
        stack_top_addr++;
        fprintf(file, "\t\tSTA\t%d\n", stack_top_addr);
    }

    codeGeneration(node->n1);
    Symbol *lhs = pop_symbol();

    switch(node->n2->nodeType) {
    case plusNode:
        emitAddCode(lhs, rhs);
        break;
    case minusNode:
        emitSubCode(lhs, rhs);
        break;
    }

    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    sym->type = EXPRESSION;
    sym->storedIn = STORED_IN_RA;

    push_symbol(sym);
}

void codegen_varNode(Node *node) {
    //Push a symbol in the symbol stack containing the variable
    //If it is an array push the expression of the index in the stack
    if(node->nodeType == varNode) {
        Symbol *symbol = st->table[searchForSymbol(INTEGER_TYPE, node->identifier)];
        symbol->storedIn = STORED_IN_MEM;
        push_symbol(symbol);
    }
    else if(node->nodeType == varExpressionNode) {
        codeGeneration(node->n1);
        Symbol *sym = pop_symbol();
        if(sym->type == EXPRESSION) {
            stack_top_addr++;
            fprintf(file, "\t\tSTA\t%d\n", stack_top_addr);
        }
        else if(sym->type == INTEGER_TYPE) {
            fprintf(file, "\t\tLDA\t%d\n", sym->address);
            stack_top_addr++;
            fprintf(file, "\t\tSTA\t%d\n", stack_top_addr);
        }
        else if(sym->type == INTEGER_ARR_TYPE) {
            fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
            fprintf(file, "\t\tCMP1\t=0=\n");
            fprintf(file, "\t\tJL\tERRMSG\n");
            stack_top_addr--;
            fprintf(file, "\t\tLDA\t%d,1\n", sym->address, stack_top_addr);
            stack_top_addr++;
            fprintf(file, "\t\tSTA\t%d\n", stack_top_addr);
        }
        else if(sym->type == INT_CONSTANT) {
            fprintf(file, "\t\tLDA\t=%d=\n", sym->value);
            stack_top_addr++;
            fprintf(file, "\t\tSTA\t%d\n", stack_top_addr);
        }

        Symbol *symbol = st->table[searchForSymbol(INTEGER_TYPE, node->identifier)];
        symbol->type = INTEGER_ARR_TYPE;
        symbol->storedIn = STORED_IN_MEM;
        push_symbol(symbol);
    }
}

void codegen_termNode(Node *node) {
    //Same as additive, just with mulop

    codeGeneration(node->n3);
    Symbol *rhs = pop_symbol();

    if(rhs->type == EXPRESSION) {
        stack_top_addr++;
        fprintf(file, "\t\tSTA\t%d\n", stack_top_addr);
    }

    codeGeneration(node->n1);
    Symbol *lhs = pop_symbol();

    switch(node->n2->nodeType) {
    case mulNode:
        emitMulCode(lhs, rhs);
        fprintf(file, "\t\tSTX\t%d\n", stack_top_addr+1);
        fprintf(file, "\t\tLDA\t%d\n", stack_top_addr+1);
        break;
    case divNode:
        emitDivCode(lhs, rhs);
        break;
    }

    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    sym->storedIn = STORED_IN_RA;
    sym->type = EXPRESSION;
    push_symbol(sym);
}

void codegen_factorNode(Node *node) {
    codeGeneration(node->n1);
}

void codegen_intLiteralNode(Node *node) {
    //Push a symbol in the symbol stack that contains the int literal

    Symbol* sym = (Symbol *)malloc(sizeof(Symbol));
    sym->value = node->number;
    sym->type = INT_CONSTANT;
    push_symbol(sym);
}
