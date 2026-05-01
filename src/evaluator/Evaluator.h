#pragma once
#include "../parser/AST.h"
#include <unordered_map>
#include <string>

class Evaluator
{
private:
    std::unordered_map<std::string, double> variables;
    std::unordered_map<std::string, double> constants;

public:
    Evaluator();
    double evaluate(ASTNode *node);
};