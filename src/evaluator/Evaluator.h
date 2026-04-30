#pragma once
#include "../parser/AST.h"

class Evaluator
{
public:
    double evaluate(ASTNode *node);
};