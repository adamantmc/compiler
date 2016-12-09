#ifndef _CODE_TEMPLATES_H
#define _CODE_TEMPLATES_H

#include "symbol.h"

void emitStartingCode(int baseaddr);
void emitPrintCode(Symbol *symbol);
void emitEndCode();

void emitMulCode(Symbol *lhs, Symbol *rhs);
void emitDivCode(Symbol *lhs, Symbol *rhs);
void emitAddCode(Symbol *lhs, Symbol *rhs);
void emitSubCode(Symbol *lhs, Symbol *rhs);

void emitCmpCode(Symbol *lhs, Symbol *rhs);

#endif
