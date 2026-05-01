#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "utility/CalcError.h"
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

        try
        {
            Lexer lexer(input);
            std::vector<Token> tokens = lexer.tokenise();

            Parser parser(tokens);
            std::unique_ptr<ASTNode> node = parser.parse();

            double result = evaluator.evaluate(node.get());

            if (std::abs(result) < 1e-12)
            {
                result = 0;
            }

            std::cout << "= " << result << std::endl;
        }
        catch (const CalcError &e)
        {
            std::cout << input << std::endl;
            std::cout << std::string(e.pos, ' ') << "^" << std::endl;
            std::cout << "Error: " << e.what() << std::endl;
        }
        catch (const std::runtime_error &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}