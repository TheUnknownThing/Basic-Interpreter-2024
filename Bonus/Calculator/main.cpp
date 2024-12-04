#include "antlr4-runtime.h"
#include "ExprLexer.h"
#include "ExprParser.h"
#include "EvalVisitor.h"
#include <exception>
#include <iostream>
#include <any>

int main(int argc, const char* argv[]) {
  std::ifstream stream;
  stream.open("../testcases/example.in");
  if (!stream.is_open()) {
    std::cerr << "Could not open file" << std::endl;
    return 1;
  }
  antlr4::ANTLRInputStream input(stream);

  ExprLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);

  ExprParser parser(&tokens);

  antlr4::tree::ParseTree *tree = parser.calc();

  EvalVisitor eval;
  std::any result = eval.visit(tree);

  try {
    int value = std::any_cast<int>(result);
    std::cout << value << std::endl;
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}