cmake_minimum_required(VERSION 3.16)
project(ScientificCalculator)

# C++ Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Setups
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Qt auto tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# CORE LIBRARY 
add_library(core STATIC
    core/lexer/Lexer.cpp
    core/parser/Parser.cpp
    core/evaluator/Evaluator.cpp
    core/calculator/Calculator.cpp
    core/types/Matrix.cpp
    core/types/Value.cpp
    core/types/Function.cpp
    core/utility/Printer.cpp
    core/utility/Utility.cpp
)

target_include_directories(core PUBLIC core)

# CLI APP 
add_executable(cli
    apps/cli/main.cpp
)

target_link_libraries(cli PRIVATE core)



# ---- GUI APP ----
add_executable(gui
    apps/gui/main.cpp
    apps/gui/MainWindow.cpp
)

set_target_properties(gui PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(gui
    PRIVATE core
    Qt6::Widgets
)
