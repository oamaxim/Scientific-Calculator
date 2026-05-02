#pragma once
#include "../parser/AST.h"
#include "../types/Matrix.h"
#include "../types/Value.h"
#include <unordered_map>
#include <string>

class Evaluator
{
private:
    std::unordered_map<std::string, Value> variables;
    std::unordered_map<std::string, double> constants;
    double asNumber(const Value &v);
    const Matrix& asMatrix(const Value &v);

public:
    Evaluator();
    Value evaluate(ASTNode *node);
    
};