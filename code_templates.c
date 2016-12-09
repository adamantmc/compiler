#include "code_templates.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *file;
extern int stack_top_addr;
extern int label_counter;

void emitStartingCode(int baseaddr) {
    fprintf(file, "PRINTER EQU\t19\n");
    fprintf(file, "PRINTM  EQU\t%d\n", baseaddr);
    fprintf(file, "PRINT   EQU\t%d\n", baseaddr+1);
    fprintf(file, "START   EQU\t%d\n", baseaddr+24+2);
    fprintf(file, "STACK   EQU\t3901\n");
    fprintf(file, "\t\tORIG\tSTART\n");
    fprintf(file, "\t\tLDA\t=45=\n");
    fprintf(file, "\t\tSTA\t%d\n", baseaddr);
}

void emitPrintCode(Symbol *symbol) {
    fprintf(file, "\n");

    if(symbol->type == INTEGER_TYPE) {
        fprintf(file, "\t\tLDA\t%d Load variable in RA\n", symbol->address);
    }
    else if(symbol->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        int label = label_counter;
        stack_top_addr--;
        fprintf(file, "\t\tLDA\t%d,1 Load variable in RA\n", symbol->address);
    }
    else if(symbol->type == INT_CONSTANT) {
        fprintf(file, "\t\tLDA\t=%d=\n", symbol->value);
    }


    fprintf(file, "\t\tCMPA\t=0=\n");
    int label = label_counter;
    label_counter++;

    fprintf(file, "\t\tCHAR\n");
    fprintf(file, "\t\tSTA\tPRINT Store rA in PRINT\n");
    fprintf(file, "\t\tSTX\tPRINT+1 Store rX in PRINT+1\n");

    fprintf(file, "\t\tJL\tPJ%d\n", label);
    fprintf(file, "\t\tOUT\tPRINT(PRINTER) Print\n");
    fprintf(file, "\t\tJMP\tPE%d\n", label);

    fprintf(file, "PJ%d\t\tNOP\n",label);
    fprintf(file, "\t\tOUT\tPRINTM(PRINTER) Print\n");

    fprintf(file, "PE%d\t\tNOP\n", label);
}

void emitMulCode(Symbol *lhs, Symbol *rhs) {
    fprintf(file, "\n");

    if(lhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tLDA\t%d Load variable in rA for mul\n",lhs->address);
    }
    else if(lhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tLDA\t%d,1\n", lhs->address);
    }
    else if(lhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tLDA\t=%d= Enter int constant in rA for mul\n", lhs->value);
    }

    if(rhs->type == EXPRESSION) {
        fprintf(file, "\t\tMUL\t%d\n", stack_top_addr);
        stack_top_addr--;
    }
    else if(rhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tMUL\t%d\n", rhs->address);
    }
    else if(rhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tMUL\t%d,1\n", rhs->address);
    }
    else if(rhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tMUL\t=%d=\n", rhs->value);
    }
}

void emitDivCode(Symbol *lhs, Symbol *rhs) {
    fprintf(file, "\n");

    if(lhs->type == EXPRESSION) {
        fprintf(file, "\t\tSTA\t%d\n", stack_top_addr+1);
        fprintf(file, "\t\tLDX\t%d\n", stack_top_addr+1);
    }
    else if(lhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tLDX\t%d\n", lhs->address);
    }
    else if(lhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tLDX\t%d,1\n", lhs->address);
    }
    else if(lhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tLDX\t=%d=\n", lhs->value);
    }

    if(rhs->type == EXPRESSION) {
        fprintf(file, "\t\tENTA\t0\n");
        fprintf(file, "\t\tDIV\t%d\n", stack_top_addr);
        stack_top_addr--;
    }
    else if(rhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tENTA\t0\n");
        fprintf(file, "\t\tDIV\t%d\n",rhs->address);
    }
    else if(rhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tENTA\t0\n");
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tDIV\t%d,1\n", rhs->address);
    }
    else if(rhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tENTA\t0\n");
        fprintf(file, "\t\tDIV\t=%d=\n",rhs->value);
    }
}

void emitAddCode(Symbol *lhs, Symbol *rhs) {
    fprintf(file, "\n");

    if(lhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tLDA\t%d\n",lhs->address);
    }
    else if(lhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tLDA\t%d,1\n", lhs->address);
    }
    else if(lhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tLDA\t=%d=\n", lhs->value);
    }

    if(rhs->type == EXPRESSION) {
        fprintf(file, "\t\tADD\t%d\n", stack_top_addr);
        stack_top_addr--;
    }
    else if(rhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tADD\t%d\n", rhs->address);
    }
    else if(rhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tADD\t%d,1\n", rhs->address);
    }
    else if(rhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tADD\t=%d=\n", rhs->value);
    }
}

void emitSubCode(Symbol *lhs, Symbol *rhs) {
    fprintf(file, "\n");

    if(lhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tLDA\t%d\n",lhs->address);
    }
    else if(lhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tLDA\t%d,1\n", lhs->address);
    }
    else if(lhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tLDA\t=%d=\n", lhs->value);
    }

    if(rhs->type == EXPRESSION) {
        fprintf(file, "\t\tSUB\t%d\n", stack_top_addr);
        stack_top_addr--;
    }
    else if(rhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tSUB\t%d\n", rhs->address);
    }
    else if(rhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tSUB\t%d,1\n", rhs->address);
    }
    else if(rhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tSUB\t=%d=\n", rhs->value);
    }
}

void emitCmpCode(Symbol *lhs, Symbol *rhs) {
    fprintf(file, "\n");

    if(lhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tLDA\t%d\n",lhs->address);
    }
    else if(lhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tLDA\t%d,1\n", lhs->address);
    }
    else if(lhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tLDA\t=%d=\n", lhs->value);
    }

    if(rhs->type == EXPRESSION) {
        fprintf(file, "\t\tCMPA\t%d\n", stack_top_addr);
        stack_top_addr--;
    }
    else if(rhs->type == INTEGER_TYPE) {
        fprintf(file, "\t\tCMPA\t%d\n", rhs->address);
    }
    else if(rhs->type == INTEGER_ARR_TYPE) {
        fprintf(file, "\t\tLD1\t%d\n", stack_top_addr);
        fprintf(file, "\t\tCMP1\t=0=\n");
        fprintf(file, "\t\tJL\tERRMSG\n");
        stack_top_addr--;
        fprintf(file, "\t\tCMPA\t%d,1\n", rhs->address);
    }
    else if(rhs->type == INT_CONSTANT) {
        fprintf(file, "\t\tCMPA\t=%d=\n", rhs->value);
    }
}

void emitEndCode() {
    fprintf(file, "\t\tJMP\tENDLABEL\n");

    fprintf(file, "ERRMSG\t\tNOP\n");
    fprintf(file, "\t\tOUT\tNEGERR(PRINTER)\n");


    fprintf(file, "ENDLABEL\t\tNOP\n");
    fprintf(file, "\t\tHLT\n");

    fprintf(file, "NEGERR\t\tALF\t\"Error\"\n");
    fprintf(file, "\t\tALF\t\": Neg\"\n");
    fprintf(file, "\t\tALF\t\"ative\"\n");
    fprintf(file, "\t\tALF\t\" inde\"\n");
    fprintf(file, "\t\tALF\t\"x in \"\n");
    fprintf(file, "\t\tALF\t\"array\"\n");
    fprintf(file, "\t\tALF\t\".    \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tALF\t\"     \"\n");
    fprintf(file, "\t\tEND\tSTART\n");
}
