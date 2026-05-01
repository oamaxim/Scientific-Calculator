#include "Evaluator.h"
#include "../parser/AST.h"
#include "../utility/CalcError.h"
#include <stdexcept>
#include <cmath>
#include <vector>

Evaluator::Evaluator()
{
    constants["pi"] = M_PI;
    constants["e"] = M_E;
}

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

        if (constants.count(var->name))
        {
            return constants[var->name];
        }
        throw std::runtime_error("Undefined variable '" + var->name + "'");
    }

    // ASSIGNMENT
    if (auto assign = dynamic_cast<AssignNode *>(node))
    {
        if (constants.count(assign->name))
        {
            throw std::runtime_error("Cannot assign to constant '" + assign->name + "'");
        }

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

    if (auto func = dynamic_cast<FunctionNode *>(node))
    {
        std::vector<double> values;
        for (auto &arg : func->args)
        {
            values.push_back(evaluate(arg.get()));
        }
        if (func->name == "sin")
            return std::sin(values[0]);
        if (func->name == "cos")
            return std::cos(values[0]);
        if (func->name == "tan")
            return std::tan(values[0]);
        if (func->name == "sqrt")
            return std::sqrt(values[0]);

        throw std::runtime_error("Unknown function '" + func->name + "'");
    }

    throw std::runtime_error("Unknown AST node");
}