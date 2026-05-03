cmake_minimum_required(VERSION 3.10)
project(ScientificCalculator)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(core
    core/lexer/Lexer.cpp
    core/parser/Parser.cpp
    core/evaluator/Evaluator.cpp
    core/calculator/Calculator.cpp
    core/types/Matrix.cpp
    core/utility/Printer.cpp
    core/utility/Utility.cpp
)

add_executable(cli apps/cli/main.cpp)
target_link_libraries(cli core)

