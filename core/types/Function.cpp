#include "Function.h"

void FunctionUtil::validateArgs(const std::vector<Value> &args, size_t expected, Validate::Args check, const std::string &name)
{
    bool failed = false;
    std::string message;

    switch (check)
    {
    case Validate::Args::Exact:
        failed = args.size() != expected;
        message = "expects exactly ";
        break;

    case Validate::Args::AtLeast:
        failed = args.size() < expected;
        message = "expects at least ";
        break;

    case Validate::Args::AtMost:
        failed = args.size() > expected;
        message = "expects at most ";
        break;
    }

    if (failed)
    {
        throw std::runtime_error(
            name +
            "() " +
            message +
            std::to_string(expected) +
            " arguments");
    }
}

void FunctionUtil::validateRange(double value, double expected, Validate::Compare op, const std::string &name)
{
    bool failed = false;
    std::string symbol;

    switch (op)
    {
    case Validate::Compare::LT:
        failed = value < expected;
        symbol = "<";
        break;

    case Validate::Compare::LTE:
        failed = value <= expected;
        symbol = "<=";
        break;

    case Validate::Compare::GT:
        failed = value > expected;
        symbol = ">";
        break;

    case Validate::Compare::GTE:
        failed = value >= expected;
        symbol = ">=";
        break;

    case Validate::Compare::NEQ:
        failed = value != expected;
        symbol = "!=";
        break;
    }

    if (failed)
    {
        throw std::runtime_error(
            name +
            " cannot be " +
            symbol +
            std::to_string(expected));
    }
}

void FunctionUtil::validateRange(double value, double lower, double upper, Validate::Compare op_l, Validate::Compare op_u, const std::string &name)
{
    bool failed_l = false;
    bool failed_u = false;
    std::string symbol_l;
    std::string symbol_u;

    switch (op_l)
    {
    case Validate::Compare::LT:
        failed_l = value < lower;
        symbol_l = "<";
        break;

    case Validate::Compare::LTE:
        failed_l = value <= lower;
        symbol_l = "<=";
        break;

    case Validate::Compare::GT:
        failed_l = value > lower;
        symbol_l = ">";
        break;

    case Validate::Compare::GTE:
        failed_l = value >= lower;
        symbol_l = ">=";
        break;
    }

    switch (op_u)
    {
    case Validate::Compare::LT:
        failed_u = value < upper;
        symbol_u = "<";
        break;

    case Validate::Compare::LTE:
        failed_u = value <= upper;
        symbol_u = "<=";
        break;

    case Validate::Compare::GT:
        failed_u = value > upper;
        symbol_u = ">";
        break;

    case Validate::Compare::GTE:
        failed_u = value >= upper;
        symbol_u = ">=";
        break;
    }

    if (failed_l || failed_u)
    {
        throw std::runtime_error(
            name +
            " must be in range " +
            std::to_string(lower) + symbol_l + " x " + symbol_u + std::to_string(upper));
    }
}

void FunctionUtil::expectPeriodic(double value, double period, const std::string &name, Validate::PeriodRule rule, double offset, double epsilon)
{
    double x = (value - offset) / period;
    double nearest = std::round(x);

    bool ok = std::abs(x - nearest) < epsilon;

    if (!ok)
    {
        if (rule == Validate::PeriodRule::MultipleOf)
        {
            throw std::runtime_error(
                name + " must be a multiple of " +
                std::to_string(period));
        }
        else
        {
            throw std::runtime_error(
                name + " must be of form " +
                std::to_string(offset) +
                " + n*" +
                std::to_string(period));
        }
    }
}
