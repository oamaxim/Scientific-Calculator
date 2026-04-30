#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "evaluator/Evaluator.h"

int main()
{
    std::string input;

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

            Evaluator evaluator;
            double result = evaluator.evaluate(node.get());

            std::cout << "= " << result << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}