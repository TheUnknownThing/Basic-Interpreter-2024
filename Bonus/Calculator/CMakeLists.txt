cmake_minimum_required(VERSION 3.22) # check your cmake version and modify this line

set(CMAKE_CXX_STANDARD 20) # c++20 is required
project(Calculator)

include_directories(src)

add_executable(code
        src/ExprLexer.h
        src/ExprLexer.cpp
        src/ExprParser.h
        src/ExprParser.cpp
        src/ExprVisitor.h
        src/ExprVisitor.cpp
        EvalVisitor.h
        main.cpp
)

if (APPLE)
        target_include_directories(code PRIVATE /opt/homebrew/include/antlr4-runtime)
        find_library(ANTLR4_LIB antlr4-runtime PATHS /opt/homebrew/opt/antlr4-cpp-runtime/lib)
        target_link_libraries(code ${ANTLR4_LIB})
else()
        target_include_directories(code PRIVATE /usr/include/antlr4-runtime)
        target_link_libraries(code antlr4-runtime)
endif()
