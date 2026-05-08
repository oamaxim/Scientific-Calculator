#include "Calculator.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

Value Calculator::evaluate(const std::string& input)
{
    Lexer lexer(input);
    auto tokens = lexer.tokenise();

    Parser parser(tokens);
    auto ast = parser.parse();

    Value answer = evaluator.evaluate(ast.get());
    evaluator.setANS(answer);
    return answer;
}

void Calculator::setAngleMode(AngleMode mode)
{
    evaluator.setAngleMode(mode);
}

AngleMode Calculator::getAngleMode() const
{
    return evaluator.getAngleMode();
}

std::string Calculator::getSymbolTable()
{
    return evaluator.symbolTableToString();
}