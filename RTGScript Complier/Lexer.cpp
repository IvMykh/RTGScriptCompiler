#include "Lexer.h"

using namespace std;

Lexer::Lexer()
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
	allFrags.push_back(NFAFragment::createNFAForString("and").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("or").addAcceptingState(Token::Type::Keyword));
	allFrags.push_back(NFAFragment::createNFAForString("struct").addAcceptingState(Token::Type::Keyword));



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
	NFAFragment nfaForIntegerLiteral =
		NFAFragment::createNFAForRange({ '+', '-' })
		.makeQuestionMark()
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9')
		.makePlus())
		.addAcceptingState(Token::Type::IntegerLiteral);

	allFrags.push_back(nfaForIntegerLiteral);

	

	// nfa for real literal;
	// [+-]?([0-9]+.[0-9]*|.[0-9]+)
	NFAFragment nfaForPlusMinus = 
		NFAFragment::createNFAForRange({ '+', '-' })
		.makeQuestionMark();

	NFAFragment nfaForfirstForm =
		NFAFragment::createNFAForRange('0', '9')
		.makePlus()
		.makeConcatenation(NFAFragment::createNFAForSingleSymbol('.'))
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makeStar());

	NFAFragment nfaForSecondForm =
		NFAFragment::createNFAForSingleSymbol('.')
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makePlus());

	NFAFragment nfaForRealLiteral =
		nfaForPlusMinus
		.makeConcatenation(NFAFragment::makeParallel({ nfaForfirstForm, nfaForSecondForm }))
		.addAcceptingState(Token::Type::RealLiteral);

	allFrags.push_back(nfaForRealLiteral);


	// for operators;
	/*
		+
		-
		*
		/
		(
		)
		=
		==
		!=
		<
		>
	*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('+').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('-').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('*').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('/').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('(').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol(')').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('=').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForString("==").addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('<').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('>').addAcceptingState(Token::Type::Operator));
	allFrags.push_back(NFAFragment::createNFAForString("!=").addAcceptingState(Token::Type::Operator));

	dfa_ = NFAFragment::makeParallel(allFrags).convertToDFA();
}

Lexer::~Lexer()
{
}
