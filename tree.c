#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

char keywords[13][20] = {"PROGRAM","BEGIN","END","WRITE","READ","IF","THEN","ELSE","ENDIF","WHILE","DO","ENDDO","VAR"};

int keywords_size = 13;

char* getTypeName(int type) {
    switch(type) {
    case typeSpecifierNode:
        return "Type Specifier";
        break;
    case expStatementNode:
        return "Expression Statement";
        break;
    case expStatementSemicolonNode:
        return "Expression Statement Semicolon";
        break;
    case compStatementNode:
        return "Compound Statement";
        break;
    case selStatementNode:
        return "Selection Statement";
        break;
    case iterStatementNode:
        return "Iteration Statement";
        break;
    case printStatementNode:
        return "Print Statement";
        break;
    case statementNode:
        return "Statement";
        break;
    case localDeclarationsNode:
        return "Local Declarations";
        break;
    case empLocalDeclarationsNode:
        return "Empty Local Declarations";
        break;
    case expAssignNode:
        return "Expression Assign";
        break;
    case expSimpleNode:
        return "Expression Simple";
        break;
    case simpleExpRelopNode:
        return "Simple Expression with Relop";
        break;
    case simpleExpNode:
        return "Simple Expression";
        break;
    case varNode:
        return "Var";
        break;
    case varExpressionNode:
        return "Var Expression";
        break;
    case addExpressionNode:
        return "Add Expression";
        break;
    case addExpressionTermNode:
        return "Add Expression Term";
        break;
    case termNode:
        return "Term";
        break;
    case termFactorNode:
        return "Term factor";
        break;
    case factorExpressionNode:
        return "Factor Expression";
        break;
    case factorVarNode:
        return "Factor var";
        break;
    case assignNode:
        return "Assign";
        break;
    case writeNode:
        return "Write";
        break;
    case readNode:
        return "Read";
        break;
    case ifNode:
        return "If";
        break;
    case whileNode:
        return "While";
        break;
    case blockNode:
        return "Block";
        break;
    case plusNode:
        return "Plus";
        break;
    case minusNode:
        return "Minus";
        break;
    case mulNode:
        return "Mul";
        break;
    case divNode:
        return "Div";
        break;
    case grNode:
        return "Greater";
        break;
    case grEqNode:
        return "Greater_Equal";
        break;
    case leNode:
        return "Less";
        break;
    case leEqNode:
        return "Less_Equal";
        break;
    case eqNode:
        return "Equal";
        break;
    case nEqNode:
        return "Not_Equal";
        break;
    case unMinusNode:
        return "Unary_Minus";
        break;
    case parNode:
        return "Parenthesis";
        break;
    case intLiteralNode:
        return "Int_Literal";
        break;
    case idNode:
        return "Id";
        break;
    case declNode:
        return "Declaration";
        break;
    case declListNode:
        return "Declare_List";
        break;
    case empDeclListNode:
        return "Empty_Declare_List";
        break;
    case statListNode:
        return "Statement_List";
        break;
    case empStatListNode:
        return "Empty_Statement_List";
        break;
    case programNode:
        return "Program_Node";
        break;
    }
    return "Undefined";
}



Node* makeNode(int type, int number, char* id, Node* node1, Node* node2, Node* node3) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->nodeType = type;
    node->n1 = node1;
    node->n2 = node2;
    node->n3 = node3;
    node->number = number;
    if(id != NULL) {
        strcpy(node->identifier,id);
    }

    return node;
}

void printTabs(int n) {

    for(int i = 0; i < n*4; i++) printf(" ");
}

void printTree(Node* node, int tabs) {
    if(node == NULL) return;
    char *t1 = getTypeName(node->nodeType);

    printTabs(tabs);
    printf("%s\n",t1);

    if(node->n1 != NULL) {
        printTree(node -> n1, tabs+1);
    }
    if(node->n2 != NULL) {
        printTree(node -> n2, tabs+1);
    }
    if(node->n3 != NULL) {
        printTree(node -> n3, tabs+1);
    }
}

void traverse(Node *node, int *error) {
    if(node == NULL) return;
    /*if(node->nodeType == idNode) {
        if(exists(st,makeSymbol(INTEGER_TYPE,node->identifier)) == -1) {
            printf("Error: variable %s has not been declared.\n",node->identifier);
            *error = 1;
        }

        //Not needed, lexer will return a token, and a syntax error will be found.
        int i;
        for(i=0; i < keywords_size; i++)
            if(strcmp(keywords[i],node->identifier) == 0) {
                printf("Error: %s is a reserved keyword and cannot be used as an identifier.",node->identifier);
                *error = 1;
            }

    }*/
    traverse(node -> n1,error);
    traverse(node -> n2,error);
    traverse(node -> n3,error);
}
