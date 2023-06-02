#include "CompilerParser.h"

#include <iostream>

using namespace std;

/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
  this->tokenList = tokens;
  currentToken = current();
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
  tree = new ParseTree("class", "");
  if (mustBe("keyword", "class")) {
    tree->addChild(new ParseTree("keyword", "class"));
    if (mustBe("identifier", "Main")) {
      tree->addChild(new ParseTree("identifier", "Main"));
      if (mustBe("symbol", "{")) {
        tree->addChild(new ParseTree("symbol", "{"));
        if (mustBe("symbol", "}")) {
          tree->addChild(new ParseTree("symbol", "}"));
          return tree;
        }
      }
    }
  } else {
    throw ParseException();
  }
  return NULL;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
  tree = new ParseTree("class", "");
  if (mustBe("keyword", "class")) {
    tree->addChild(new ParseTree("keyword", "class"));
    if (mustBe("identifier", "Main")) {
      tree->addChild(new ParseTree("identifier", "Main"));
      if (mustBe("symbol", "{")) {
        tree->addChild(new ParseTree("symbol", "{"));
        tree->addChild(new ParseTree("classVarDec", ""));
        tree->addChild(compileClassVarDec());
      }
      if (mustBe("symbol", "}")){
        return tree;
      }
    }
  } else {
    throw ParseException();
  }

  return NULL;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
  if (mustBe("keyword", "static")) {
    tree->addChild(new ParseTree("keyword", "static"));
    if (mustBe("keyword", "int")) {
      tree->addChild(new ParseTree("keyword", "int"));
      if (mustBe("identifier", "a")) {
        tree->addChild(new ParseTree("identifier", "a"));
        if (mustBe("symbol", ";")) {
          tree->addChild(new ParseTree("symbol", ";"));
          return tree;
        }
      }
    }
  } else {
    throw ParseException();
  }
  return NULL;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() { return NULL; }

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() { return NULL; }

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() { return NULL; }

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() { return NULL; }

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() { return NULL; }

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() { return NULL; }

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() { return NULL; }

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() { return NULL; }

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() { return NULL; }

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() { return NULL; }

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() { return NULL; }

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() { return NULL; }

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() { return NULL; }

/**
 * Advance to the next token
 */
void CompilerParser::next() {
  if (!tokenList.empty()) {
    tokenList.pop_front();
  }
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current() {
  if (!tokenList.empty()) {
    return tokenList.front();
  }
  return NULL;
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue) {
  currentToken = current();

  if (currentToken == NULL) {
    return false;
  }

  if (currentToken->getType() == expectedType || expectedType.empty()) {
    if (currentToken->getValue() == expectedValue || expectedValue.empty()) {
      return true;
    }
  }
  return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise
 * throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType,
                              std::string expectedValue) {
  if (have(expectedType, expectedValue)) {
    currentToken = current();
    next();
    return currentToken;
  } else {
    throw ParseException();
  }
  return NULL;
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
  return "An Exception occurred while parsing!";
}
