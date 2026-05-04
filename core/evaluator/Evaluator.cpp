#include "Evaluator.h"
#include "parser/AST.h"
#include "types/Matrix.h"
#include "utility/Utility.h"
#include <stdexcept>
#include <cmath>
#include <vector>

Evaluator::Evaluator()
{
    constants["pi"] = M_PI;
    constants["e"] = M_E;

    constants["tau"] = 2 * M_PI;
    constants["phi"] = (1 + std::sqrt(5)) / 2;
}

Value Evaluator::evaluate(ASTNode *node)
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

        Value val = evaluate(assign->value.get());

        variables[assign->name] = val;

        return val;
    }

    // BINARY
    if (auto bin = dynamic_cast<BinaryNode *>(node))
    {
        Value leftV = evaluate(bin->left.get());
        Value rightV = evaluate(bin->right.get());

        // Both Numbers
        if (std::holds_alternative<double>(leftV) &&
            std::holds_alternative<double>(rightV))
        {

            double left = std::get<double>(leftV);
            double right = std::get<double>(rightV);

            switch (bin->op)
            {
            case '+':
                return left + right;
            case '-':
                return left - right;
            case '*':
                return left * right;
            case '/':
                if (right == 0)
                    throw std::runtime_error("Division by zero");
                return left / right;
            case '^':
                return std::pow(left, right);
            }
        }

        // Both Matrix
        if (std::holds_alternative<Matrix>(leftV) &&
            std::holds_alternative<Matrix>(rightV))
        {

            const Matrix &A = std::get<Matrix>(leftV);
            const Matrix &B = std::get<Matrix>(rightV);

            switch (bin->op)
            {
            case '+':
                return Matrix::add(A, B);
            case '-':
                return Matrix::subtract(A, B);
            case '*':
                return Matrix::multiply(A, B);
            case '/':
                return Matrix::divide(A, B);
            }
        }

        // Number and Matrix
        if (std::holds_alternative<double>(leftV) &&
            std::holds_alternative<Matrix>(rightV) &&
            bin->op == '*')
        {
            double s = std::get<double>(leftV);
            const Matrix &M = std::get<Matrix>(rightV);

            return Matrix::scalarMultiply(M, s);
        }

        // Matrix and Number
        if (std::holds_alternative<Matrix>(leftV) &&
            std::holds_alternative<double>(rightV))
        {
            const Matrix &M = std::get<Matrix>(leftV);
            double s = std::get<double>(rightV);

            switch (bin->op)
            {
            case '*':
                return Matrix::scalarMultiply(M, s);
            case '^':
                return Matrix::power(M, (int)s);
            }
        }
    }

    if (auto func = dynamic_cast<FunctionNode *>(node))
    {
        std::vector<double> values;
        for (auto &arg : func->args)
        {
            values.push_back(asNumber(evaluate(arg.get())));
        }
        // Trig
        if (func->name == "sin")
            return std::sin(toRadians(values[0], angleMode));
        if (func->name == "cos")
            return std::cos(toRadians(values[0], angleMode));
        if (func->name == "tan")
            return std::tan(toRadians(values[0], angleMode));

        // Reciprocal Trig
        if (func->name == "cosec")
            return 1 / std::sin(toRadians(values[0], angleMode));
        if (func->name == "sec")
            return 1 / std::cos(toRadians(values[0], angleMode));
        if (func->name == "cot")
            return 1 / std::tan(toRadians(values[0], angleMode));

        // Inverse Trig
        if (func->name == "arcsin")
            return fromRadians(std::asin(values[0]), angleMode);
        if (func->name == "arccos")
            return fromRadians(std::acos(values[0]), angleMode);
        if (func->name == "arctan")
            return fromRadians(std::atan(values[0]), angleMode);

        // Inverse Reciprocal Trig
        if (func->name == "arccsc")
            return fromRadians(std::asin(1 / values[0]), angleMode);
        if (func->name == "arcsec")
            return fromRadians(std::acos(1 / values[0]), angleMode);
        if (func->name == "arccot")
            return fromRadians(std::atan(1 / values[0]), angleMode);

        // Hyperbolic
        if (func->name == "sinh")
            return std::sinh(values[0]);
        if (func->name == "cosh")
            return std::cosh(values[0]);
        if (func->name == "tanh")
            return std::tanh(values[0]);

        // Inverse Hyperbolic
        if (func->name == "arcsinh")
            return std::asinh(values[0]);
        if (func->name == "arccosh")
            return std::acosh(values[0]);
        if (func->name == "arctanh")
            return std::atanh(values[0]);

        // Matricies
        if (func->name == "mat")
        {
            auto &args = func->args;
            if (args.size() < 2)
            {
                throw std::runtime_error("Matrix requires rows and columns");
            }

            int r = (int)asNumber(evaluate(args[0].get()));
            int c = (int)asNumber(evaluate(args[1].get()));

            Matrix m(r, c);

            int expected = r * c;

            if ((int)func->args.size() - 2 != expected)
            {
                throw std::runtime_error("Incorrect number of matrix values");
            }

            for (int i = 0; i < expected; i++)
            {
                m.data[i] = asNumber(evaluate(args[i + 2].get()));
            }

            return m;
        }

        if (func->name == "det")
        {
            if (func->args.size() != 1)
            {
                throw std::runtime_error("det() expects 1 matrix argument");
            }

            Matrix M = asMatrix(evaluate(func->args[0].get()));

            return Matrix::determinant(M);
        }

        if (func->name == "transpose")
        {
            if (func->args.size() != 1)
            {
                throw std::runtime_error("transpose() expects 1 matrix argument");
            }
            Matrix M = asMatrix(evaluate(func->args[0].get()));
            return Matrix::transpose(M);
        }

        if (func->name == "inv")
        {
            Matrix m = asMatrix(evaluate(func->args[0].get()));
            return Matrix::inverse(m);
        }

        // Others
        if (func->name == "sqrt")
            return std::sqrt(values[0]);

        if (func->name == "abs")
            return std::abs(values[0]);

        throw std::runtime_error("Unknown function '" + func->name + "'");
    }

    throw std::runtime_error("Unknown AST node");
}

double Evaluator::asNumber(const Value &v)
{
    if (!std::holds_alternative<double>(v))
    {
        throw std::runtime_error("Expected number");
    }
    return std::get<double>(v);
}

const Matrix &Evaluator::asMatrix(const Value &v)
{
    if (!std::holds_alternative<Matrix>(v))
    {
        throw std::runtime_error("Expected matrix");
    }
    return std::get<Matrix>(v);
}

void Evaluator::setAngleMode(AngleMode mode)
{
    angleMode = mode;
}

AngleMode Evaluator::getAngleMode() const
{
    return angleMode;
}


double Evaluator::toRadians(double x, AngleMode mode)
{
    if (mode == AngleMode::DEG)
        return x * M_PI / 180.0;
    return x;
}

double Evaluator::fromRadians(double x, AngleMode mode)
{
    if (mode == AngleMode::DEG)
        return x * 180.0 / M_PI;
    return x;
}