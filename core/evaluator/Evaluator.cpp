#include "Evaluator.h"
#include "parser/AST.h"
#include "types/Matrix.h"
#include "types/Value.h"
#include "types/Function.h"
#include "utility/Utility.h"
#include <utility/Printer.h>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <sstream>

Evaluator::Evaluator()
{
    // Constants
    constants["pi"] = M_PI;
    constants["π"] = M_PI;
    constants["e"] = M_E;

    constants["tau"] = 2 * M_PI;
    constants["phi"] = (1 + std::sqrt(5)) / 2;
    constants["ans"] = 0.0;

    // Functions

    // Trig
    functions["sin"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "sin");

        double x = ValueUtil::asNumber(args[0]);
        return std::sin(toRadians(x, angleMode));
    };

    functions["cos"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "cos");

        double x = ValueUtil::asNumber(args[0]);
        return std::cos(toRadians(x, angleMode));
    };

    functions["tan"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "tan");

        double x = toRadians(ValueUtil::asNumber(args[0]), angleMode);

        FunctionUtil::expectPeriodic(
            x,
            (angleMode == AngleMode::DEG) ? 180.0 : M_PI,
            "tan",
            Validate::PeriodRule::OffsetMultipleOf,
            (angleMode == AngleMode::DEG) ? 90.0 : M_PI_2,
            1e-9);

        return std::tan(x);
    };

    // Reciprocal Trig

    functions["csc"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "csc");

        double x = toRadians(ValueUtil::asNumber(args[0]), angleMode);
        FunctionUtil::expectPeriodic(
            x,
            (angleMode == AngleMode::DEG) ? 180.0 : M_PI,
            "csc",
            Validate::PeriodRule::MultipleOf,
            0,
            1e-9);

        return 1 / std::sin(x);
    };

    functions["sec"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "sec");

        double x = toRadians(ValueUtil::asNumber(args[0]), angleMode);

        FunctionUtil::expectPeriodic(
            x,
            (angleMode == AngleMode::DEG) ? 180.0 : M_PI,
            "sec",
            Validate::PeriodRule::OffsetMultipleOf,
            (angleMode == AngleMode::DEG) ? 90.0 : M_PI_2,
            1e-9);

        return 1 / std::cos(x);
    };

    functions["cot"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "cot");

        double x = toRadians(ValueUtil::asNumber(args[0]), angleMode);

        FunctionUtil::expectPeriodic(
            x,
            (angleMode == AngleMode::DEG) ? 180.0 : M_PI,
            "cot",
            Validate::PeriodRule::MultipleOf,
            0,
            1e-9);

        return 1 / std::tan(x);
    };

    // Inverse Trig

    functions["asin"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "asin");

        double x = ValueUtil::asNumber(args[0]);
        FunctionUtil::validateRange(
            x,
            -1,
            1,
            Validate::Compare::LTE,
            Validate::Compare::GTE,
            "asin");

        return fromRadians(std::asin(x), angleMode);
    };

    functions["acos"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "acos");

        double x = ValueUtil::asNumber(args[0]);
        FunctionUtil::validateRange(
            x,
            -1,
            1,
            Validate::Compare::LTE,
            Validate::Compare::GTE,
            "acos");

        return fromRadians(std::acos(x), angleMode);
    };

    functions["atan"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "atan");

        double x = ValueUtil::asNumber(args[0]);

        return fromRadians(std::atan(x), angleMode);
    };

    // Inverse Reciprocal Trig

    functions["acsc"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "acsc");

        double x = ValueUtil::asNumber(args[0]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::NEQ, "x");

        return fromRadians(std::asin(1 / x), angleMode);
    };

    functions["asec"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "asec");

        double x = ValueUtil::asNumber(args[0]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::NEQ, "x");

        return fromRadians(std::acos(1 / x), angleMode);
    };

    functions["acot"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "acot");

        double x = ValueUtil::asNumber(args[0]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::NEQ, "x");

        return fromRadians(std::atan(1 / x), angleMode);
    };

    // Hyperbolic

    functions["sinh"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "sinh");

        return std::sinh(ValueUtil::asNumber(args[0]));
    };

    functions["cosh"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "cosh");

        return std::cosh(ValueUtil::asNumber(args[0]));
    };

    functions["tanh"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "tanh");

        return std::tanh(ValueUtil::asNumber(args[0]));
    };

    // Inverse Hyperbolic

    functions["asinh"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "asinh");

        return std::asinh(ValueUtil::asNumber(args[0]));
    };

    functions["acosh"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "acosh");
        return std::acosh(ValueUtil::asNumber(args[0]));
    };

    functions["atanh"] = [this](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "atanh");
        return std::atanh(ValueUtil::asNumber(args[0]));
    };

    // Matrices

    functions["mat"] = [](const std::vector<Value> &args)
    {
        if (args.size() < 2)
        {
            throw std::runtime_error("Matrix requires rows and columns");
        }

        int r = (int)ValueUtil::asNumber(args[0]);
        int c = (int)ValueUtil::asNumber(args[1]);

        Matrix m(r, c);

        int expected = r * c;

        if (args.size() - 2 != expected)
        {
            throw std::runtime_error("Incorrect number of matrix values");
        }

        for (int i = 0; i < expected; i++)
        {
            m.data[i] = ValueUtil::asNumber(args[i + 2]);
        }

        return m;
    };

    functions["det"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "det");

        return Matrix::determinant(ValueUtil::asMatrix(args[0]));
    };

    functions["transpose"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "transpose");

        return Matrix::transpose(ValueUtil::asMatrix(args[0]));
    };

    functions["inv"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "inv");

        return Matrix::inverse(ValueUtil::asMatrix(args[0]));
    };

    functions["identity"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "identity");

        return Matrix::identity(ValueUtil::asNumber(args[0]));
    };

    functions["sqrt"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "sqrt");

        double x = ValueUtil::asNumber(args[0]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::LT, "Radicand");

        return Value(std::sqrt(x));
    };

    functions["root"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 2, Validate::Args::Exact, "root");

        double n = ValueUtil::asNumber(args[0]);
        double x = ValueUtil::asNumber(args[1]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::LT, "Index");
        FunctionUtil::validateRange(n, 0, Validate::Compare::LTE, "Radicand");

        return std::pow(x, 1 / n);
    };

    functions["abs"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "abs");
        return std::abs(ValueUtil::asNumber(args[0]));
    };

    functions["log"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 2, Validate::Args::Exact, "log");

        double n = ValueUtil::asNumber(args[0]);
        double x = ValueUtil::asNumber(args[1]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::GT, "Value");

        return logbase(x, n);
    };

    functions["ln"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "ln");

        double x = ValueUtil::asNumber(args[0]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::GT, "Value");

        return std::log(x);
    };

    functions["log10"] = [](const std::vector<Value> &args)
    {
        FunctionUtil::validateArgs(args, 1, Validate::Args::Exact, "log10");

        double x = ValueUtil::asNumber(args[0]);

        FunctionUtil::validateRange(x, 0, Validate::Compare::GT, "Value");

        return std::log10(x);
    };
};

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

        switch (bin->op)
        {
        case '+':
            return ValueUtil::add(leftV, rightV);
        case '-':
            return ValueUtil::subtract(leftV, rightV);
        case '*':
            return ValueUtil::multiply(leftV, rightV);
        case '/':
            return ValueUtil::divide(leftV, rightV);
        case '^':
            return ValueUtil::power(leftV, rightV);
        }
    }

    if (auto func = dynamic_cast<FunctionNode *>(node))
    {
        std::vector<Value> args;

        for (auto &arg : func->args)
        {
            args.push_back(
                evaluate(arg.get()));
        }

        auto it = functions.find(func->name);

        if (it == functions.end())
        {
            throw std::runtime_error(
                "Unknown function '" +
                func->name + "'");
        }

        return it->second(args);
    }

    throw std::runtime_error("Unknown AST node");
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

void Evaluator::setANS(Value v)
{
    constants["ans"] = v;
}

std::string Evaluator::symbolTableToString() const
{
    std::ostringstream oss;

    oss << "Constants:\n";
    for (const auto &[name, value] : constants)
    {
        oss << name << " = " << Printer::toString(value) << "\n";
    }

    oss << "\nVariables:\n";
    for (const auto &[name, value] : variables)
    {
        oss << name << " = " << Printer::toString(value) << "\n";
    }

    return oss.str();
}