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
  if (var == "LET" || var == "PRINT" || var == "INPUT" || var == "GOTO" ||
      var == "IF" || var == "END") {
    std::cout << "SYNTAX ERROR" << std::endl;
    return;
  }
  if (scanner.nextToken() != "=") {
    std::cout << "SYNTAX ERROR" << std::endl;
    return;
  }
  try {
    exp = parseExp(scanner);
  } catch (ErrorException &ex) {
    if (ex.getMessage() == "DIVIDE BY ZERO") {
      std::cout << "DIVIDE BY ZERO" << std::endl;
      return;
    } else if (ex.getMessage() == "VARIABLE NOT DEFINED") {
      std::cout << "VARIABLE NOT DEFINED" << std::endl;
      return;
    } else {
      std::cout << "SYNTAX ERROR" << std::endl;
    }
    return;
  }
  if (scanner.hasMoreTokens()) {
    std::cout << "SYNTAX ERROR" << std::endl;
    return;
  }
}

LetStmt::~LetStmt() { delete exp; }

void LetStmt::execute(EvalState &state, Program &program) {
  state.setValue(var, exp->eval(state));
}

PrintStmt::PrintStmt(TokenScanner &scanner) : exp(nullptr) {
  exp = parseExp(scanner);

  if (scanner.hasMoreTokens()) {
    std::cout << "SYNTAX ERROR" << std::endl;
  }
}

PrintStmt::~PrintStmt() {
  if (exp != nullptr) {
    delete exp;
  }
}

void PrintStmt::execute(EvalState &state, Program &program) {
  try {
    std::cout << exp->eval(state) << std::endl;
  } catch (ErrorException &ex) {
    std::cout << ex.getMessage() << std::endl;
  }
}

InputStmt::InputStmt(TokenScanner &scanner) {
  var = scanner.nextToken();
  if (scanner.hasMoreTokens()) {
    std::cout << "SYNTAX ERROR" << std::endl;
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
    std::cout << "SYNTAX ERROR" << std::endl;
  }
}

void GotoStmt::execute(EvalState &state, Program &program) {
  if (program.findLine(lineNumber)) {
    program.setCurrentLine(lineNumber - 1);
  } else {
    error("LINE NUMBER ERROR");
  }
}

IfStmt::IfStmt(TokenScanner &scanner) {
  try {
    // std::string token = scanner.nextToken();
    // std::cout << token << std::endl;
    // scanner.saveToken(token);
    exp1 = parseExp(scanner);
    op = scanner.nextToken();
    exp2 = parseExp(scanner);
  } catch (...) {
    std::cout << "SYNTAX ERROR" << std::endl;
    return;
  }
  if (scanner.nextToken() != "THEN") {
    std::cout << "SYNTAX ERROR" << std::endl;
    return;
  }
  lineNumber = stringToInt(scanner.nextToken());
  if (scanner.hasMoreTokens()) {
    std::cout << "SYNTAX ERROR" << std::endl;
  }
}

void IfStmt::execute(EvalState &state, Program &program) {
  if (!program.findLine(lineNumber)) {
    error("LINE NUMBER ERROR");
    return;
  }
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