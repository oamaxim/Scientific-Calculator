#pragma once
#include <unordered_map>
#include <string>
#include "../parser/AST.h"

class Evaluator
{
private:
    std::unordered_map<std::string, double> variables;

public:
    double evaluate(ASTNode *node);
};