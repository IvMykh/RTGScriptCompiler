#pragma once
#include <string>
#include <vector>
#include <map>
#include "Instructions.h"
#include "Token.h"
using namespace std;
using namespace Instructions;

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
		string tokenValue = "";
		switch (token->getTokenType())
		{
		case Token::Type::Keyword:			tokenValue = ((KeywordToken*)token)->getValue(); break;
		case Token::Type::Identifier:		tokenValue = ((IdentifierToken*)token)->getValue(); break;
		case Token::Type::IntegerLiteral:	tokenValue = to_string(((IntegerLiteralToken*)token)->getValue()); break;
		case Token::Type::RealLiteral:		tokenValue = to_string(((RealLiteralToken*)token)->getValue()); break;
		case Token::Type::Operator:			tokenValue = ((OperatorToken*)token)->getValue(); break;
		default:							throw exception("Unexpected token type\n");
			break;
		}
		return "Error in token '" + tokenValue + "'. " + message + ".";
	}
};

class Parser
{
	vector<Token*> tokens;
	multimap <string, Expressions::Storage*> storages;

	typedef vector<Token*>::iterator TokenIter;
public:
	Parser(vector<Token*> tkns);

	bool isOperator(TokenIter& iter);
	bool isOperator(TokenIter& iter, string op);
	bool isKeyword(TokenIter& iter);
	bool isKeyword(TokenIter& iter, string kw);
	bool isIdentifier(TokenIter& iter);
	string getIdentifier(TokenIter& iter);
	Expressions::Storage::Type* isTypeName(TokenIter& iter);
	Expressions::Storage* isStorage(TokenIter& iter);
	Expressions::Storages::Literal* isIntegerLiteral(TokenIter& iter);
	Expressions::Storages::Literal* isRealLiteral(TokenIter& iter);
	Expressions::Storages::Literal* isBoolLiteral(TokenIter& iter);
	Expressions::Storages::Literal* isVecMatLiteral(TokenIter& iter);
	Expressions::Storages::Literal* isLiteral(TokenIter& iter);
	Expressions::Operators::BinaryOperator* isBinaryOperator(TokenIter& iter);
	Block* isBlock(TokenIter& iter);
	Instruction* isInstruction(TokenIter& iter);
	bool isEmptyInstruction(TokenIter& iter);
	Definition* isDefinition(TokenIter& iter);
	Expressions::Operators::Binary::Assignment* isAssignment(TokenIter& iter);
	Expression* isExpression(TokenIter& iter);

	Block* parse();

	multimap <string, Expressions::Storage*>* getStorages();

	~Parser();
};