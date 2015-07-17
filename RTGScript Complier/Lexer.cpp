#include "Lexer.h"

#include <fstream>

using namespace std;

Lexer::Lexer():
	dfa_()
{
	vector<NFAFragment> allFrags;
	
	// for keywords;
	/*
		if
		and
		or
		else
		struct
	*/
	allFrags.push_back(NFAFragment::createNFAForString("if").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("else").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("and").addAcceptingState(Token::Type::Operator)); // operator!
	allFrags.push_back(NFAFragment::createNFAForString("or").addAcceptingState(Token::Type::Operator)); // operator!
	allFrags.push_back(NFAFragment::createNFAForString("not").addAcceptingState(Token::Type::Operator)); // operator!
	allFrags.push_back(NFAFragment::createNFAForString("struct").addAcceptingState(Token::Type::Keyword));

	/*type idendifiers*/
	allFrags.push_back(NFAFragment::createNFAForString("bool").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("int").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("uint").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("float").addAcceptingState(Token::Type::Keyword));

	allFrags.push_back(NFAFragment::createNFAForString("vec2").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("vec3").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("vec4").addAcceptingState(Token::Type::Keyword));
	
	allFrags.push_back(NFAFragment::createNFAForString("ivec2").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("ivec3").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("ivec4").addAcceptingState(Token::Type::Keyword));

	allFrags.push_back(NFAFragment::createNFAForString("mat2").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("mat3").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("mat4").addAcceptingState(Token::Type::Keyword));

	allFrags.push_back(NFAFragment::createNFAForString("quat").addAcceptingState(Token::Type::Keyword));

	/*1D, 2D, 3D, Cube*/
	allFrags.push_back(NFAFragment::createNFAForString("1D").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("2D").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("3D").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("Cube").addAcceptingState(Token::Type::Keyword));

	/*in, out, extern*/
	allFrags.push_back(NFAFragment::createNFAForString("in").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("out").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("extern").addAcceptingState(Token::Type::Keyword));

	/*true, false*/
	allFrags.push_back(NFAFragment::createNFAForString("true").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("false").addAcceptingState(Token::Type::Keyword));

	/*rgba*/
	allFrags.push_back(NFAFragment::createNFAForString("red").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("green").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("blue").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("alpha").addAcceptingState(Token::Type::Keyword));


	// for identifiers;
	// [_a-zA-Z][_a-zA-z0-9]*
	NFAFragment nfaForaToz = NFAFragment::createNFAForRange('a', 'z');
	NFAFragment nfaForAToZ = NFAFragment::createNFAForRange('A', 'Z');
	NFAFragment nfaFor0To9 = NFAFragment::createNFAForRange('0', '9');
	NFAFragment nfaForUnderscore = NFAFragment::createNFAForSingleSymbol('_');

	NFAFragment nfaForIDs =
		NFAFragment::makeParallel({ nfaForaToz, nfaForAToZ, nfaForUnderscore })
		.makeConcatenation(
		NFAFragment::makeParallel({ nfaForaToz, nfaForAToZ, nfaForUnderscore, nfaFor0To9 }).makeStar()
		)
		.addAcceptingState(Token::Type::Identifier);
	
	allFrags.push_back(nfaForIDs);



	// for integer literals;
	// [+-]?[0-9]+
	//NFAFragment nfaForIntegerLiteral =
	//	NFAFragment::createNFAForRange({ '+', '-' })
	//	.makeQuestionMark()
	//	.makeConcatenation(NFAFragment::createNFAForRange('0', '9')
	//	.makePlus())
	//	.addAcceptingState(Token::Type::IntegerLiteral);
	//
	//allFrags.push_back(nfaForIntegerLiteral);

	// for integer literals;
	// [0-9]+
	NFAFragment nfaForIntegerLiteral =
		NFAFragment::createNFAForRange('0', '9')
		.makePlus()
		.addAcceptingState(Token::Type::IntegerLiteral);
	
	allFrags.push_back(nfaForIntegerLiteral);


	// nfa for real literal;
	// [+-]?([0-9]+.[0-9]*|.[0-9]+)	
	//NFAFragment nfaForPlusMinus = 
	//	NFAFragment::createNFAForRange({ '+', '-' })
	//	.makeQuestionMark();
	//
	//NFAFragment nfaForfirstForm =
	//	NFAFragment::createNFAForRange('0', '9')
	//	.makePlus()
	//	.makeConcatenation(NFAFragment::createNFAForSingleSymbol('.'))
	//	.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makeStar());
	//
	//NFAFragment nfaForSecondForm =
	//	NFAFragment::createNFAForSingleSymbol('.')
	//	.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makePlus());
	//
	//NFAFragment nfaForRealLiteral =
	//	nfaForPlusMinus
	//	.makeConcatenation(NFAFragment::makeParallel({ nfaForfirstForm, nfaForSecondForm }))
	//	.addAcceptingState(Token::Type::RealLiteral);
	
	// nfa for real literal;
	// ([0-9]+.[0-9]*|.[0-9]+)	
	NFAFragment nfaForfirstForm =
		NFAFragment::createNFAForRange('0', '9')
		.makePlus()
		.makeConcatenation(NFAFragment::createNFAForSingleSymbol('.'))
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makeStar());
	
	NFAFragment nfaForSecondForm =
		NFAFragment::createNFAForSingleSymbol('.')
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makePlus());
	
	NFAFragment nfaForRealLiteral =
		NFAFragment::makeParallel({ nfaForfirstForm, nfaForSecondForm })
		.addAcceptingState(Token::Type::RealLiteral);

	allFrags.push_back(nfaForRealLiteral);


	// for operators;

	/*arithmetical operators
		+
		-
		*
		/
	*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('+').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('-').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('*').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('/').addAcceptingState(Token::Type::Operator));

	/*logical operators
	<
	>
	==
	!=
	&&
	||
	!
	*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('<').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('>').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForString("==").addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForString("!=").addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForString("&&").addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForString("||").addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('!').addAcceptingState(Token::Type::Operator));

	/*assignment*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('=').addAcceptingState(Token::Type::Operator));

	/*semicolon*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol(';').addAcceptingState(Token::Type::Operator));

	/*dot*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('.').addAcceptingState(Token::Type::Operator));
	
	/*brackets*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('(').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol(')').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('[').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol(']').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('{').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('}').addAcceptingState(Token::Type::Operator));

	/*comma*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol(',').addAcceptingState(Token::Type::Operator));

	this->dfa_ = NFAFragment::makeParallel(allFrags).convertToDFA();

	
	// serialization;
	ofstream outputFile("Serialized result DFA.txt");
	
	this->dfa_.serialize(outputFile);
}

Lexer::Lexer(const string& fileName):
	dfa_()
{
	ifstream inputFile(fileName);
	if (!inputFile)
	{
		string message = "Cannot open file \"" + fileName + "\"";
		throw exception(message.c_str());
	}

	this->dfa_.deserialize(inputFile);
}

vector<Token*> Lexer::tokenize(const string& inputString) const
{
	return this->dfa_.tokenize(inputString);
}

Lexer::~Lexer()
{
}
