#include <iostream>
#include "lexer/Lexer.h"

int main()
{
    std::string input;
    std::getline(std::cin, input);

    Lexer lexer(input);
    auto tokens = lexer.tokenise();

    for (auto token : tokens)
    {
        std::cout << static_cast<int>(token.type)
                  << " "
                  << token.value
                  << std::endl;
    }

    return 0;
}