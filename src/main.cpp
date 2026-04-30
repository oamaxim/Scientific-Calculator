#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

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
            double result = parser.parse();

            std::cout << "= " << result << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}