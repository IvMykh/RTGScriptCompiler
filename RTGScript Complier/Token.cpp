#include "Token.h"

using namespace std;

// class Token definition;
Token::Token(const Type tokenType) :
	tokenType_(tokenType)
{
}

const Token::Type Token::getTokenType() const
{
	return tokenType_;
}

const string Token::getTokenTypeString() const
{
	switch (tokenType_)
	{
	case Type::Keyword:			return "Keyword";
	case Type::Identifier:		return "Identifier";
	case Type::IntegerLiteral:	return "IntegerLiteral";
	case Type::RealLiteral:		return "RealLiteral";
	case Type::Operator:		return "Operator";
	default:					throw exception("Unexpected token type!");
	}
}

Token::~Token()
{
}

Token* Token::createToken(const Type tokenType, const string& valueStr)
{
	switch (tokenType)
	{
	case Type::Keyword:			return (Token*)new KeywordToken(Type::Keyword, valueStr);
	case Type::Identifier:		return (Token*)new IdentifierToken(Type::Identifier, valueStr);
	case Type::IntegerLiteral:	return (Token*)new IntegerLiteralToken(Type::IntegerLiteral, valueStr);
	case Type::RealLiteral:		return (Token*)new RealLiteralToken(Type::RealLiteral, valueStr);
	case Type::Operator:		return (Token*)new OperatorToken(Type::Operator, valueStr);
	default:					throw exception("Unexpected token type");
		break;
	}

	//if (tokenString == "Keyword")			return (Token*)new KeywordToken(Type::Keyword, valueStr);
	//if (tokenString == "Identifier")		return (Token*)new IdentifierToken(Type::Identifier, valueStr);
	//if (tokenString == "IntegerLiteral")	return (Token*)new IntegerLiteralToken(Type::IntegerLiteral, valueStr);
	//if (tokenString == "RealLiteral")		return (Token*)new RealLiteralToken(Type::RealLiteral, valueStr);
	//if (tokenString == "Operator")			return (Token*)new OperatorToken(Type::Operator, valueStr);
	//
	//throw exception("Unexpected token type");
}

Token::Type Token::getTypeByString(const std::string& tokenString)
{
	if (tokenString == "Keyword")			return Type::Keyword;
	if (tokenString == "Identifier")		return Type::Identifier;
	if (tokenString == "IntegerLiteral")	return Type::IntegerLiteral;
	if (tokenString == "RealLiteral")		return Type::RealLiteral;
	if (tokenString == "Operator")			return Type::Operator;

	throw exception("Unexpected token type");
}



// class KeywordToken definitions;
KeywordToken::KeywordToken(const Type tokenType, const std::string valueStr) :
	Token(tokenType),
	value_(valueStr)
{
}

const string& KeywordToken::getValue() const
{
	return value_;
}



// class IdentifierToken definitions;
IdentifierToken::IdentifierToken(const Type tokenType, const std::string valueStr) :
	Token(tokenType),
	value_(valueStr)
{
}

const string& IdentifierToken::getValue() const
{
	return value_;
}



// class IntegerLiteralToken definitions;
IntegerLiteralToken::IntegerLiteralToken(const Type tokenType, const std::string valueStr) :
	Token(tokenType),
	value_(atoi(valueStr.c_str()))
{
}

const int IntegerLiteralToken::getValue() const
{
	return value_;
}


// class RealLiteralToken definitions;
RealLiteralToken::RealLiteralToken(const Type tokenType, const std::string valueStr) :
	Token(tokenType),
	value_(stof(valueStr.c_str()))
{
}

const float RealLiteralToken::getValue() const
{
	return value_;
}



// class OperatorTokendefinitions;
OperatorToken::OperatorToken(const Type tokenType, const std::string valueStr) :
	Token(tokenType),
	value_(valueStr)
{
}

const string& OperatorToken::getValue() const
{
	return value_;
}