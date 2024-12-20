/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include "parser.hpp"

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner &scanner, bool allowAssignment) {
    try {
        Expression *exp = readE(scanner, 0, allowAssignment);
        if (scanner.hasMoreTokens()) {
            error("SYNTAX ERROR");
        }
        return exp;
    } catch (ErrorException &ex) {
        error("SYNTAX ERROR");
        return nullptr;
    }
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner &scanner, int prec, bool allowAssignment) {
    try {
        Expression *exp = readT(scanner);
        std::string token;
        while (true) {
            token = scanner.nextToken();
            int newPrec = precedence(token);
            if (newPrec <= prec) break;
            if (token == "=" && !allowAssignment) {
                error("SYNTAX ERROR");
            }
            Expression *rhs = readE(scanner, newPrec, allowAssignment);
            exp = new CompoundExp(token, exp, rhs);
        }
        scanner.saveToken(token);
        return exp;
    } catch (ErrorException &ex) {
        error("SYNTAX ERROR");
        return nullptr;
    }
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner &scanner) {
    try {
        std::string token = scanner.nextToken();
        TokenType type = scanner.getTokenType(token);
        if (type == WORD) return new IdentifierExp(token);
        if (type == NUMBER) return new ConstantExp(stringToInteger(token));
        if (token == "-") {
            std::string nextToken = scanner.nextToken();
            if (nextToken == "+" || nextToken == "-") {
                error("SYNTAX ERROR");
            }
            scanner.saveToken(nextToken);
            return new CompoundExp(token, new ConstantExp(0), readE(scanner));
        }
        if (token != "(") error("Illegal term in expression");
        Expression *exp = readE(scanner);
        if (scanner.nextToken() != ")") {
            error("Unbalanced parentheses in expression");
        }
        return exp;
    } catch (ErrorException &ex) {
        error("SYNTAX ERROR");
        return nullptr; // This line will never be reached, but it's here to satisfy the compiler.
    }
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(std::string token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}