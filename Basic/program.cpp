/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/error.hpp"

Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
  sourceLines.clear();
  parsedStatements.clear();
  lineNumbers.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
  if (sourceLines.find(lineNumber) != sourceLines.end()) {
    sourceLines.erase(lineNumber);
  }
  sourceLines[lineNumber] = line;
  lineNumbers.insert(lineNumber);
}

void Program::removeSourceLine(int lineNumber) {
  if (sourceLines.find(lineNumber) != sourceLines.end()) {
    sourceLines.erase(lineNumber);
    lineNumbers.erase(lineNumber);
  }
}

std::string Program::getSourceLine(int lineNumber) {
  try {
    return sourceLines.at(lineNumber);
  } catch (std::out_of_range &ex) {
    return "";
  }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
  if (sourceLines.find(lineNumber) == sourceLines.end()) {
    error("LINE NUMBER ERROR");
  } else {
    if (parsedStatements.find(lineNumber) != parsedStatements.end()) {
      parsedStatements.erase(lineNumber);
    }
    parsedStatements[lineNumber] = stmt;
  }
}

Statement *Program::getParsedStatement(int lineNumber) {
  try {
    return parsedStatements.at(lineNumber);
  } catch (std::out_of_range &ex) {
    return nullptr;
  }
}

int Program::getFirstLineNumber() {
  if (lineNumbers.empty()) {
    return -1;
  } else {
    return *lineNumbers.begin();
  }
}

int Program::getNextLineNumber(int lineNumber) {
  auto it = lineNumbers.upper_bound(lineNumber);
  if (it == lineNumbers.end()) {
    return -1;
  } else {
    return *it;
  }
}

void Program::listAllLines() {
  for (int lineNumber : lineNumbers) {
    std::cout << sourceLines[lineNumber] << std::endl;
  }
}

void Program::setCurrentLine(int lineNumber) {
  currentLine = lineNumber;
}

int Program::getCurrentLine() {
  return currentLine;
}

int Program::nextLine(){
  if (lineNumbers.empty() || currentLine == -1) {
    return -1;
  }
  return getNextLineNumber(currentLine);
}