/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include <cctype>
#include <iostream>
#include <string>

/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
  EvalState state;
  Program program;
  // cout << "Stub implementation of BASIC" << endl;
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
    state.Clear();
    program.clear();
    exit(0);
  } else if (line == "CLEAR") {
    state.Clear();
    program.clear();
  } else if (line == "HELP") {
    std::cout << "This is a simple BASIC interpreter." << std::endl;
    std::cout << "  LIST: List all the lines of the program." << std::endl;
    std::cout << "  RUN: Run the program." << std::endl;
    std::cout << "  CLEAR: Clear the program." << std::endl;
    std::cout << "  QUIT: Quit the interpreter." << std::endl;
  } else if (line == "LIST") {
    program.listAllLines();
  } else if (line == "RUN") {
    int firstLine = program.getFirstLineNumber();
    if (firstLine == -1) {
      std::cout << "SYNTAX ERROR" << std::endl;
    } else {
      int lineNumber = firstLine;
      program.setCurrentLine(lineNumber);
      while (lineNumber != -1) {
        Statement *stmt = program.getParsedStatement(lineNumber);
        if (stmt == nullptr) {
          std::cout << "SYNTAX ERROR" << std::endl;
        }
        stmt->execute(state, program);
        lineNumber = program.nextLine();
        program.setCurrentLine(lineNumber);
      }
    }
  } else {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.ignoreComments();
    scanner.setInput(line);
    std::string token = scanner.nextToken();
    if (scanner.getTokenType(token) == NUMBER) {
      int lineNumber = stringToInteger(token);
      scanner.scanStrings();
      token = scanner.nextToken();
      Statement *stmt = nullptr;
      scanner.scanNumbers();
      if (token == "REM") {
        stmt = new RemStmt(scanner);
      } else if (token == "LET") {
        stmt = new LetStmt(scanner);
      } else if (token == "PRINT") {
        stmt = new PrintStmt(scanner);
      } else if (token == "INPUT") {
        stmt = new InputStmt(scanner);
      } else if (token == "GOTO") {
        stmt = new GotoStmt(scanner);
      } else if (token == "IF") {
        stmt = new IfStmt(scanner);
      } else if (token == "END") {
        stmt = new EndStmt();
      } else if (token == "") {
        // delete the line
        program.removeSourceLine(lineNumber);
      } else {
        std::cout << "SYNTAX ERROR" << std::endl;
      }
      if (stmt != nullptr) {
        program.addSourceLine(lineNumber, line);
        program.setParsedStatement(lineNumber, stmt);
      }
    } else if (scanner.getTokenType(token) == WORD) {
      if (token == "LET") {
        LetStmt *stmt = new LetStmt(scanner);
        stmt->execute(state, program);
        delete stmt;
      } else if (token == "PRINT") {
        PrintStmt *stmt = new PrintStmt(scanner);
        stmt->execute(state, program);
        delete stmt;
      } else if (token == "INPUT") {
        InputStmt *stmt = new InputStmt(scanner);
        stmt->execute(state, program);
        delete stmt;
      } else {
        std::cout << "SYNTAX ERROR" << std::endl;
      }
    } else {
      std::cout << "SYNTAX ERROR" << std::endl;
    }
  }
}