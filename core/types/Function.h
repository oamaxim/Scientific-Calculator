#pragma once

#include <functional>
#include <unordered_map>
#include "Value.h"
#include <cmath>

using Function =
    std::function<Value(const std::vector<Value> &)>;

namespace Validate
{
    enum class Compare
    {
        LT,
        LTE,
        GT,
        GTE,
        NEQ,
    };

    enum class Args
    {
        Exact,
        AtLeast,
        AtMost
    };

    enum class PeriodRule
    {
        MultipleOf,
        OffsetMultipleOf
    };
}

namespace FunctionUtil
{
    void validateArgs(const std::vector<Value> &args, size_t expected, Validate::Args check, const std::string &name);

    void validateRange(double value, double expected, Validate::Compare op, const std::string &name);

    void validateRange(double value, double lower, double upper, Validate::Compare op_l, Validate::Compare op_u, const std::string &name);

    void expectPeriodic(double value, double period, const std::string& name, Validate::PeriodRule rule, double offset, double epsilon);
}
