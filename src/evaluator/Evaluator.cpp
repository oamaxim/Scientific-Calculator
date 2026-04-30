#include "Evaluator.h"
#include "../parser/AST.h"
#include <stdexcept>

double Evaluator::evaluate(ASTNode *node)
{
    if (auto num = dynamic_cast<NumberNode *>(node))
    {
        return num->value;
    }
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
        }
    }

    throw std::runtime_error("Unknown AST node");
}