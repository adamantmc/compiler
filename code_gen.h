#ifndef _CODE_GEN_H
#define _CODE_GEN_H

#include "tree.h"

void codeGeneration(Node *node);

void codegen_programNode(Node *node);

void codegen_declListNode(Node *node);

void codegen_localDeclarationsNode(Node *node);

void codegen_declarationNode(Node *node);

void codegen_statListNode(Node *node);

void codegen_expressionStatementNode(Node *node);

void codegen_compoundStatementNode(Node *node);

void codegen_selectionStatementNode(Node *node);

void codegen_iterationStatementNode(Node *node);

void codegen_printStatementNode(Node *node);

void codegen_expressionAssignNode(Node *node);

void codegen_simpleExpressionNode(Node *node);

void codegen_additiveExpressionNode(Node *node);

void codegen_varNode(Node *node);

void codegen_termNode(Node *node);

void codegen_factorNode(Node *node);

void codegen_intLiteralNode(Node *node);

#endif
