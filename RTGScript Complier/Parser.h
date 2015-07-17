#pragma once
#include <string>
#include <vector>
#include "ParserClasses.h"
#include "Token.h"
using namespace std;

class ParserError
{
	Token* token;
	string message;
public:
	ParserError(Token* tkn, string msg) :
		token(tkn),
		message(msg)
	{}
	string what()
	{
		return "Error in token '" + token->getTokenTypeString() + "'. " + message + ".";
	}
};

class Parser
{
	vector<Token*> tokens;
	typedef vector<Token*>::iterator TokenIter;
public:
	Parser(vector<Token*> tkns) :
		tokens(tkns)
	{}

	bool isOperator(TokenIter& iter);
	bool isOperator(TokenIter& iter, string op);
	bool isKeyword(TokenIter& iter);
	bool isKeyword(TokenIter& iter, string kw);
	bool isIdentifier(TokenIter& iter);
	string getIdentifier(TokenIter& iter);
	Variable* isVariable(TokenIter& iter);
	IntegerLiteral* isIntegerLiteral(TokenIter& iter);
	RealLiteral* isRealLiteral(TokenIter& iter);
	BoolLiteral* isBoolLiteral(TokenIter& iter);
	Literal* isLiteral(TokenIter& iter);
	Function* isFunctionToken(TokenIter& iter);
	Operator* isOperatorToken(TokenIter& iter);
	Block* isBlock(TokenIter& iter);
	Instruction* isInstruction(TokenIter& iter);
	Declaration* isDeclaration(TokenIter& iter); // + Definition
	IfStatement* isIfStatement(TokenIter& iter); // + IfElseStatement
	Assignement* isAssignement(TokenIter& iter);
	bool isEmptyInstruction(TokenIter& iter);
	Qualifier* isQualifier(TokenIter& iter);
	InOutQualifier* isInOutQualifier(TokenIter& iter);
	Expression* isExpression(TokenIter& iter);
	VariableType* isTypeName(TokenIter& iter);

	Block* parse();
};