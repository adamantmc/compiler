#ifndef _TREE_H
#define _TREE_H

#define assignNode 23731
#define writeNode 23732
#define readNode 23733
#define ifNode 23734
#define whileNode 23735
#define blockNode 23736
#define plusNode 23737
#define minusNode 23738
#define mulNode 23739
#define divNode 23740
#define grNode 23741
#define grEqNode 23742
#define leNode 23743
#define leEqNode 23744
#define eqNode 23745
#define nEqNode 23746
#define unMinusNode 23747
#define parNode 23748
#define intLiteralNode 23749
#define idNode 23750
#define declNode 23751
#define declListNode 23752
#define empDeclListNode 23753
#define statListNode 23754
#define empStatListNode 23755
#define programNode 23756
#define typeSpecifierNode 23757
#define expStatementNode 23758
#define expStatementSemicolonNode 23759
#define compStatementNode 23760
#define selStatementNode 23761
#define iterStatementNode 23762
#define printStatementNode 23763
#define statementNode 23764
#define localDeclarationsNode 23765
#define empLocalDeclarationsNode 23766
#define expAssignNode 23767
#define expSimpleNode 23768
#define simpleExpRelopNode 23769
#define simpleExpNode 23780
#define varNode 23771
#define varExpressionNode 23772
#define addExpressionNode 23773
#define addExpressionTermNode 23774
#define termNode 23775
#define termFactorNode 23776
#define factorExpressionNode 23777
#define factorVarNode 23778

typedef struct _Node Node;
struct _Node {
    int nodeType;
    int number;
    char identifier[256];
    Node* n1;
    Node* n2;
    Node* n3;
};

Node *makeNode(int type, int number, char *id, Node *n1, Node *n2, Node *n3);
void traverse(Node *node, int *error);
void printTree(Node *node, int tabs);

#endif
