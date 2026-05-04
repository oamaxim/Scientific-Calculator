#pragma once
#include "parser/AST.h"
#include "types/Matrix.h"
#include "types/Value.h"
#include <unordered_map>
#include <string>

enum class AngleMode
{
    RAD,
    DEG
};

class Evaluator
{
private:
    std::unordered_map<std::string, Value> variables;
    std::unordered_map<std::string, double> constants;

    double asNumber(const Value &v);
    const Matrix &asMatrix(const Value &v);
    static double toRadians(double x, AngleMode mode);
    static double fromRadians(double x, AngleMode mode);

public:
    Evaluator();
    Value evaluate(ASTNode *node);

    AngleMode angleMode = AngleMode::RAD;
    void setAngleMode(AngleMode mode);

    AngleMode getAngleMode() const;
};

