#include "Evaluator.h"
#include "../parser/AST.h"
#include <stdexcept>
#include <cmath>

double Evaluator::evaluate(ASTNode *node)
{
    // NUMBER
    if (auto num = dynamic_cast<NumberNode *>(node))
    {
        return num->value;
    }

    // VARIABLE
    if (auto var = dynamic_cast<VariableNode *>(node))
    {
        if (variables.count(var->name))
        {
            return variables[var->name];
        }
        throw std::runtime_error("Undefined variable: " + var->name);
    }

    // ASSIGNMENT
    if (auto assign = dynamic_cast<AssignNode *>(node))
    {
        double val = evaluate(assign->value.get());
        variables[assign->name] = val;
        return val;
    }

    // BINARY
    if (auto bin = dynamic_cast<BinaryNode *>(node))
    {
        double left = evaluate(bin->left.get());
        double right = evaluate(bin->right.get());

        switch (bin->op)
        {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            return left / right;
        case '^':
            return std::pow(left, right);
        }
    }

    throw std::runtime_error("Unknown AST node");
}