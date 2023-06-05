#include "CompilerParser.h"

#include <iostream>

using namespace std;

/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
  this->tokenList = tokens;
  currentToken = tokens.front();
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
  tree = new ParseTree("class", "keyword");
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
  ParseTree* tree = new ParseTree("class", "");
  currentToken = mustBe("keyword", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("identifier", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("symbol", "{");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  // if the next value isnt a }
  while (current()->getValue() != "}") {
    if (current()->getValue() == "static" || current()->getValue() == "field") {
      tree->addChild(compileClassVarDec());
    }
    if (current()->getValue() == "function" ||
        current()->getValue() == "method" ||
        current()->getValue() == "constructor") {
      tree->addChild(compileSubroutine());
    }
  }
  currentToken = mustBe("symbol", "}");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  return tree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
  ParseTree* tree = new ParseTree("classVarDec", "");
  currentToken = mustBe("keyword", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("keyword", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("identifier", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  if (current()->getValue() == ",") {
    currentToken = mustBe("symbol", ",");
    tree->addChild(
        new ParseTree(currentToken->getType(), currentToken->getValue()));
    currentToken = mustBe("identifier", "");
    tree->addChild(
        new ParseTree(currentToken->getType(), currentToken->getValue()));
  }
  currentToken = mustBe("symbol", ";");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  return tree;
}
/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
  ParseTree* tree = new ParseTree("subroutine", "");
  currentToken = mustBe("keyword", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));

  if (current()->getType() == "keyword") {
    currentToken = mustBe("keyword", "");
    tree->addChild(
        new ParseTree(currentToken->getType(), currentToken->getValue()));
  } else if (current()->getType() == "identifier") {
    currentToken = mustBe("identifier", "");
    tree->addChild(
        new ParseTree(currentToken->getType(), currentToken->getValue()));
  }
  currentToken = mustBe("identifier", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("symbol", "(");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));

  if (current()->getValue() != ")") {
    tree->addChild(compileParameterList());
  }

  currentToken = mustBe("symbol", ")");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("symbol", "{");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  if (current()->getValue() != "}") {
    tree->addChild(compileSubroutineBody());
  }
  currentToken = mustBe("symbol", "}");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  return tree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
  ParseTree* tree = new ParseTree("parameterList", "");
  currentToken = mustBe("keyword", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("identifier", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  if (current() == NULL) {
    return tree;
  }
  while (current()->getType() == ",") {
    currentToken = mustBe("symbol", ",");
    tree->addChild(
        new ParseTree(currentToken->getType(), currentToken->getValue()));
    if (current()->getType() == "keyword") {
      currentToken = mustBe("keyword", "");
      tree->addChild(
          new ParseTree(currentToken->getType(), currentToken->getValue()));
    } else if (current()->getType() == "identifier") {
      currentToken = mustBe("identifier", "");
      tree->addChild(
          new ParseTree(currentToken->getType(), currentToken->getValue()));
    }

    if (current()->getType() == "identifier") {
      currentToken = mustBe("identifier", "");
      tree->addChild(
          new ParseTree(currentToken->getType(), currentToken->getValue()));
    }

    if (current() == NULL) {
      break;
    }
  }
  return tree;
};

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
  ParseTree* tree = new ParseTree("subroutineBody", "");
  currentToken = mustBe("symbol", "{");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  // if (current()->getValue() != "}" && current()->getValue() == )
  compileVarDec();
  compileStatements();
  currentToken = mustBe("symbol", "}");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  return tree;
  throw ParseException();
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
  ParseTree* tree = new ParseTree("varDec", "");
  currentToken = mustBe("keyword", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("keyword", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  currentToken = mustBe("identifier", "");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  while (current()->getType() == ",") {
    currentToken = mustBe("symbol", ",");
    tree->addChild(
        new ParseTree(currentToken->getType(), currentToken->getValue()));
    if (current()->getType() == "keyword") {
      currentToken = mustBe("keyword", "");
      tree->addChild(
          new ParseTree(currentToken->getType(), currentToken->getValue()));
    } else if (current()->getType() == "identifier") {
      currentToken = mustBe("identifier", "");
      tree->addChild(
          new ParseTree(currentToken->getType(), currentToken->getValue()));
    }

    if (current()->getType() == "identifier") {
      currentToken = mustBe("identifier", "");
      tree->addChild(
          new ParseTree(currentToken->getType(), currentToken->getValue()));
    }

    if (current() == NULL) {
      break;
    }
  }
  currentToken = mustBe("symbol", ";");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));

  return tree;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() { return NULL; }

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */

ParseTree* CompilerParser::compileLet() {
  ParseTree* tree = new ParseTree("letStatement", "");
  if (mustBe("keyword", "let")) {
    tree->addChild(new ParseTree("keyword", "let"));
    if (mustBe("identifier", "a")) {
      tree->addChild(new ParseTree("identifier", "a"));
      if (mustBe("symbol", "=")) {
        tree->addChild(new ParseTree("symbol", "="));
        if (mustBe("keyword", "skip")) {
          tree->addChild(new ParseTree("keyword", "skip"));
          if (mustBe("symbol", ";")) {
            tree->addChild(new ParseTree("symbol", ";"));
            return tree;
          }
        }
      }
    }
  } else {
    throw ParseException();
  }
  return NULL;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
  throw ParseException();
  return NULL;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
  throw ParseException();
  return NULL;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
  throw ParseException();
  return NULL;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
  ParseTree* tree = new ParseTree("returnStatement", "");
  currentToken = mustBe("keyword", "return");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  if (current()->getValue() == "skip") {
    tree->addChild(compileExpression());
  } else {
    currentToken = mustBe("keyword", "");
    tree->addChild(
        new ParseTree(currentToken->getType(), currentToken->getValue()));
  }
  currentToken = mustBe("symbol", ";");
  tree->addChild(
      new ParseTree(currentToken->getType(), currentToken->getValue()));
  return tree;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
  ParseTree* tree = new ParseTree("expression", "");
  currentToken = mustBe("keyword", "skip");
  tree->addChild(new ParseTree("keyword", "skip"));
  return tree;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
  throw ParseException();
  return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
  throw ParseException();
  return NULL;
}

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
};
