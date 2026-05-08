#pragma once
#include <string>
#include "types/Value.h"
#include "evaluator/Evaluator.h"

class Calculator
{
private:
    Evaluator evaluator;

public:
    Value evaluate(const std::string& input);
    void setAngleMode(AngleMode mode);
    AngleMode getAngleMode() const;
    std::string getSymbolTable();

};