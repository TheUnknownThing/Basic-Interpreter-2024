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

//todo

RemStmt::RemStmt(TokenScanner &scanner) {
    //todo
}

void RemStmt::execute(EvalState &state, Program &program) {
    //todo
}

LetStmt::LetStmt(TokenScanner &scanner) {
    //todo
}

LetStmt::~LetStmt() {
    //todo
}

void LetStmt::execute(EvalState &state, Program &program) {
    //todo
}

PrintStmt::PrintStmt(TokenScanner &scanner) {
    //todo
}

PrintStmt::~PrintStmt() {
    //todo
}

void PrintStmt::execute(EvalState &state, Program &program) {
    //todo
}

InputStmt::InputStmt(TokenScanner &scanner) {
    //todo
}

void InputStmt::execute(EvalState &state, Program &program) {
    //todo
}

GotoStmt::GotoStmt(TokenScanner &scanner) {
    //todo
}

void GotoStmt::execute(EvalState &state, Program &program) {
    //todo
}

IfStmt::IfStmt(TokenScanner &scanner) {
    //todo
}

void IfStmt::execute(EvalState &state, Program &program) {
    //todo
}

EndStmt::EndStmt() {
    //todo
}

void EndStmt::execute(EvalState &state, Program &program) {
    //todo
}

int stringToInt(std::string str) {
    return std::stoi(str);
}
