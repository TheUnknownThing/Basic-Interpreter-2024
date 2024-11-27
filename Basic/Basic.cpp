/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */


void processLine(std::string line, Program &program, EvalState &state) {
    // Support control sequence: LIST, RUN, CLEAR, QUIT, HELP
    if (line == "QUIT") {
        exit(0);
    } else if (line == "CLEAR") {
        program.clear();
    } else if (line == "HELP") {
        std::cout << "This is a simple BASIC interpreter." << std::endl;
        std::cout << "  LIST: List all the lines of the program." << std::endl;
        std::cout << "  RUN: Run the program." << std::endl;
        std::cout << "  CLEAR: Clear the program." << std::endl;
        std::cout << "  QUIT: Quit the interpreter." << std::endl;
    } else if (line == "LIST") {
        int firstLine = program.getFirstLineNumber();
        if (firstLine == -1) {
            std::cout << "No lines in the program." << std::endl;
        } else {
            program.listAllLines();
        }
    } else if (line == "RUN") {
        int firstLine = program.getFirstLineNumber();
        if (firstLine == -1) {
            std::cout << "No lines in the program." << std::endl;
        } else {
            int lineNumber = firstLine;
            while (lineNumber != -1) {
                Statement *stmt = program.getParsedStatement(lineNumber);
                if (stmt == nullptr) {
                    error("LINE NUMBER ERROR");
                }
                stmt->execute(state, program);
                lineNumber = program.getNextLineNumber(lineNumber);
            }
        }
    } else {
        // if the line begins with a number, add it to the program
        // if the line not begin with a number, it is an expression
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(line);
        if (scanner.getTokenType(scanner.nextToken()) == NUMBER) {
            int lineNumber = stringToInteger(scanner.nextToken());
            program.addSourceLine(lineNumber, line);
        } else {
            // parse LET, PRINT and INPUT statement
            
        }
    }
}

