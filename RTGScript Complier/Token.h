#pragma once
#include <string>

class KeywordToken;
class IdentifierToken;
class IntegerLiteralToken;
class RealLiteralToken;
class OperatorToken;



// TODO: implement getValue() for each derived type;
class Token
{
public:
	enum class Type // priority!;
	{
		None = 0,
		Keyword = 5,
		Identifier = 4,
		IntegerLiteral = 3,
		RealLiteral = 2,
		Operator = 1
	};

private:
	Type tokenType_;

public:
	Token(const Type tokenType);

	const Type getTokenType() const;
	const std::string getTokenTypeString() const;

	virtual ~Token() = 0;

	// statics:
	static Token* createToken(const Type tokenType, const std::string& valueStr);
	static Type getTypeByString(const std::string& tokenString);
};



class KeywordToken
	:Token
{
	friend class Token;

	std::string value_;
	KeywordToken(const Type tokenType, const std::string valueStr);

public:
	const std::string& getValue() const;
};



class IdentifierToken
	:Token
{
	friend class Token;

	std::string value_;
	IdentifierToken(const Type tokenType, const std::string valueStr);

public:
	const std::string& getValue() const;
};



class IntegerLiteralToken
	:Token
{
	friend class Token;

	int value_;
	IntegerLiteralToken(const Type tokenType, const std::string valueStr);

public:
	const int getValue() const;
};



class RealLiteralToken
	:Token
{
	friend class Token;

	float value_;
	RealLiteralToken(const Type tokenType, const std::string valueStr);

public:
	const float getValue() const;
};



class OperatorToken
	:Token
{
	friend class Token;

	std::string value_;
	OperatorToken(const Type tokenType, const std::string valueStr);

public:
	const std::string& getValue() const;
};
