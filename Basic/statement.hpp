/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &program) = 0;

};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class RemStmt : public Statement {
public:
    RemStmt(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
};

class LetStmt : public Statement {
public:
    LetStmt(TokenScanner &scanner);
    ~LetStmt();
    void execute(EvalState &state, Program &program) override;
private:
    Expression *exp;
    std::string var;
};

class PrintStmt : public Statement {
public:
    PrintStmt(TokenScanner &scanner);
    ~PrintStmt();
    void execute(EvalState &state, Program &program) override;
private:
    Expression *exp;
};

class InputStmt : public Statement {
public:
    InputStmt(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
private:
    std::string var;
};

class GotoStmt : public Statement {
public:
    GotoStmt(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
private:
    int lineNumber;
};

class IfStmt : public Statement {
public:
    IfStmt(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
private:
    Expression *exp1, *exp2;
    std::string op;
    int lineNumber;
};

class EndStmt : public Statement {
public:
    EndStmt();
    void execute(EvalState &state, Program &program) override;
};

#endif
