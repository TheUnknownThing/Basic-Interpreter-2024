/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"

/* Implementation of the Statement class */

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
  exp = parseExp(scanner);
  if (scanner.hasMoreTokens()) {
    error("SYNTAX ERROR");
  }
}

LetStmt::~LetStmt() { delete exp; }

void LetStmt::execute(EvalState &state, Program &program) {
  state.setValue(var, exp->eval(state));
}

PrintStmt::PrintStmt(TokenScanner &scanner) {
  exp = parseExp(scanner);
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
  std::cout << " ? ";
  std::cin >> input;
  state.setValue(var, stringToInt(input));
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
  // IF exp cmp exp THEN n
  exp1 = parseExp(scanner);
  op = scanner.nextToken();
  exp2 = parseExp(scanner);
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

EndStmt::EndStmt() {}

void EndStmt::execute(EvalState &state, Program &program) {
  program.setCurrentLine(-1);
}

int stringToInt(std::string str) { return std::stoi(str); }
