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
#include "statement.hpp"
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

bool isValidNumber(const std::string &input);

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
    std::cout << "Yet another basic interpreter" << std::endl;
  } else if (line == "LIST") {
    program.listAllLines();
  } else if (line == "RUN") {
    int firstLine = program.getFirstLineNumber();

    int lineNumber = firstLine;
    program.setCurrentLine(lineNumber);
    while (lineNumber != -1) {
      Statement *stmt = program.getParsedStatement(lineNumber);
      if (stmt == nullptr) {
        std::cout << "SYNTAX ERROR" << std::endl;
      }
      try {
        stmt->execute(state, program);
      } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
        break;
      }
      lineNumber = program.nextLine();
      program.setCurrentLine(lineNumber);
    }

  } else {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.ignoreComments();
    scanner.setInput(line);
    std::string token = scanner.nextToken();
    if (scanner.getTokenType(token) == NUMBER) {
      int lineNumber;
      if (isValidNumber(token)) {
        lineNumber = std::stoi(token);
        scanner.scanStrings();
        token = scanner.nextToken();
        Statement *stmt = nullptr;
        scanner.scanNumbers();
        try {
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
            stmt = new IfStmt(scanner, line);
          } else if (token == "END") {
            stmt = new EndStmt(scanner);
          } else if (token == "") {
            // delete the line
            program.removeSourceLine(lineNumber);
          } else {
            error("SYNTAX ERROR");
          }
          if (stmt != nullptr) {
            program.addSourceLine(lineNumber, line);
            program.setParsedStatement(lineNumber, stmt);
          }
        } catch (ErrorException &ex) {
          std::cout << ex.getMessage() << std::endl;
          if (stmt != nullptr) {
            delete stmt;
          }
        }
      } else {
        error("SYNTAX ERROR");
      }

    } else if (scanner.getTokenType(token) == WORD) {
      Statement *stmt = nullptr;
      try {
        if (token == "LET") {
          stmt = new LetStmt(scanner);
          stmt->execute(state, program);
        } else if (token == "PRINT") {
          stmt = new PrintStmt(scanner);
          stmt->execute(state, program);
        } else if (token == "INPUT") {
          stmt = new InputStmt(scanner);
          stmt->execute(state, program);
        } else {
          error("SYNTAX ERROR");
        }
        if (stmt != nullptr) {
          delete stmt;
        }
      } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
        if (stmt != nullptr) {
          delete stmt;
        }
      }
    } else {
      std::cout << "SYNTAX ERROR" << std::endl;
    }
  }
}

bool isValidNumber(const std::string &input) {
  if (input.empty())
    return false;

  for (size_t i = 0; i < input.length(); ++i) {
    if (!isdigit(input[i]))
      return false;
  }

  return true;
}