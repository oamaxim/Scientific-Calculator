#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "utility/Utility.h"
#include "utility/Printer.h"
#include "evaluator/Evaluator.h"

int main()
{
    std::string input;
    Evaluator evaluator;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit")
            break;

        if (input == "deg")
        {
            evaluator.setAngleMode(AngleMode::DEG);
            std::cout << "Switched to DEG\n";
            continue;
        }

        if (input == "rad")
        {
            evaluator.setAngleMode(AngleMode::RAD);
            std::cout << "Switched to RAD\n";
            continue;
        }

        if (input == "mode")
        {
            if (evaluator.getAngleMode() == AngleMode::DEG)
                std::cout << "= DEG\n";
            else
                std::cout << "= RAD\n";
            continue;
        }

        try
        {
            Lexer lexer(input);
            std::vector<Token> tokens = lexer.tokenise();

            Parser parser(tokens);
            std::unique_ptr<ASTNode> node = parser.parse();

            Value result = evaluator.evaluate(node.get());

            Printer::printValue(result);
        }
        catch (const CalcError &e)
        {
            Printer::printCalcError(input, e);
        }
        catch (const std::runtime_error &e)
        {
            Printer::printRuntimeError(e);
        }
    }

    return 0;
}