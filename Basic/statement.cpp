/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "Utils/error.hpp"

/* Implementation of the Statement class */

bool isDecimal(const std::string &input);

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

RemStmt::RemStmt(TokenScanner &scanner) {}

void RemStmt::execute(EvalState &state, Program &program) {
  // do nothing
}

LetStmt::LetStmt(TokenScanner &scanner) {
  var = scanner.nextToken();
  if (scanner.nextToken() != "=") {
    error("SYNTAX ERROR");
  }
  try {
    exp = parseExp(scanner);
  } catch (...) {
    error("SYNTAX ERROR");
  }
  if (scanner.hasMoreTokens()) {
    error("SYNTAX ERROR");
  }
}

LetStmt::~LetStmt() { delete exp; }

void LetStmt::execute(EvalState &state, Program &program) {
  state.setValue(var, exp->eval(state));
}

PrintStmt::PrintStmt(TokenScanner &scanner) {
  try {
    exp = parseExp(scanner);
  } catch (...) {
    error("SYNTAX ERROR");
  }
  if (scanner.hasMoreTokens()) {
    error("SYNTAX ERROR");
  }
}

PrintStmt::~PrintStmt() { delete exp; }

void PrintStmt::execute(EvalState &state, Program &program) {
  std::cout << exp->eval(state) << std::endl;
}

InputStmt::InputStmt(TokenScanner &scanner) {
  var = scanner.nextToken();
  if (scanner.hasMoreTokens()) {
    error("SYNTAX ERROR");
  }
}

void InputStmt::execute(EvalState &state, Program &program) {
  std::string input;
  int value;
  while (true) {
    std::cout << " ? ";
    std::getline(std::cin, input);
    if (input.empty()) {
      std::cout << "INVALID NUMBER" << std::endl;
    } else {
      if (isDecimal(input)) {
        value = stringToInt(input);
        break;
      } else {
        std::cout << "INVALID NUMBER" << std::endl;
      }
    }
  }

  try {
    state.setValue(var, value);
  } catch (...) {
    std::cout << "INVALID NUMBER" << std::endl;
  }
}

GotoStmt::GotoStmt(TokenScanner &scanner) {
  lineNumber = stringToInt(scanner.nextToken());
  if (scanner.hasMoreTokens()) {
    error("SYNTAX ERROR");
  }
}

void GotoStmt::execute(EvalState &state, Program &program) {
  program.setCurrentLine(lineNumber);
}

IfStmt::IfStmt(TokenScanner &scanner) {
  try {
    exp1 = parseExp(scanner);
    op = scanner.nextToken();
    exp2 = parseExp(scanner);
  } catch (...) {
    error("SYNTAX ERROR");
  }
  if (scanner.nextToken() != "THEN") {
    error("SYNTAX ERROR");
  }
  lineNumber = stringToInt(scanner.nextToken());
  if (scanner.hasMoreTokens()) {
    error("SYNTAX ERROR");
  }
}

void IfStmt::execute(EvalState &state, Program &program) {
  if (op == "=" && exp1->eval(state) == exp2->eval(state)) {
    program.setCurrentLine(lineNumber);
  } else if (op == "<" && exp1->eval(state) < exp2->eval(state)) {
    program.setCurrentLine(lineNumber);
  } else if (op == ">" && exp1->eval(state) > exp2->eval(state)) {
    program.setCurrentLine(lineNumber);
  } else if (op == "<=" && exp1->eval(state) <= exp2->eval(state)) {
    program.setCurrentLine(lineNumber);
  } else if (op == ">=" && exp1->eval(state) >= exp2->eval(state)) {
    program.setCurrentLine(lineNumber);
  } else if (op == "<>" && exp1->eval(state) != exp2->eval(state)) {
    program.setCurrentLine(lineNumber);
  }
}

IfStmt::~IfStmt() {
  delete exp1;
  delete exp2;
}

EndStmt::EndStmt() {}

void EndStmt::execute(EvalState &state, Program &program) {
  program.setCurrentLine(-1);
}

bool isDecimal(const std::string &input) {
  if (input.empty())
    return false;

  size_t start = 0;
  if (input[0] == '+' || input[0] == '-')
    start = 1;

  for (size_t i = start; i < input.length(); ++i) {
    if (!isdigit(input[i]))
      return false;
  }

  return true;
}

int stringToInt(std::string str) { return std::stoi(str); }