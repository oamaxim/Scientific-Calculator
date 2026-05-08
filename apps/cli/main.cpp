#include <iostream>
#include "../../core/calculator/Calculator.h"
#include "../../core/utility/Utility.h"
#include "../../core/utility/Printer.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::string input;
    Calculator calc;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit")
            break;

        if (input == "deg")
        {
            calc.setAngleMode(AngleMode::DEG);
            std::cout << "Switched to DEG" << std::endl;
            continue;
        }

        if (input == "rad")
        {
            calc.setAngleMode(AngleMode::RAD);
            std::cout << "Switched to RAD" << std::endl;
            continue;
        }

        if (input == "mode")
        {
            if (calc.getAngleMode() == AngleMode::DEG)
                std::cout << "= DEG" << std::endl;
            else
                std::cout << "= RAD" << std::endl;
            continue;
        }

        if (input == "var/con")
        {
            std::cout << calc.getSymbolTable() << std::endl;
            continue;
        }

        try
        {
            Value result = calc.evaluate(input);

            Printer::printValue(result);
        }
        catch (const CalcError &e)
        {
            std::cout << Printer::formatCalcError(input, e) << std::endl;
        }
        catch (const std::runtime_error &e)
        {
            std::cout << Printer::formatRuntimeError(e) << std::endl;
        }
    }

    return 0;
}