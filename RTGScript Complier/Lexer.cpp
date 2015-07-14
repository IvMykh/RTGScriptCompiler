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
	allFrags.push_back(NFAFragment::createNFAForString("if").addAcceptingState("Keyword"));
	allFrags.push_back(NFAFragment::createNFAForString("else").addAcceptingState("Keyword"));
	allFrags.push_back(NFAFragment::createNFAForString("and").addAcceptingState("Keyword"));
	allFrags.push_back(NFAFragment::createNFAForString("or").addAcceptingState("Keyword"));
	allFrags.push_back(NFAFragment::createNFAForString("struct").addAcceptingState("Keyword"));



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
		.addAcceptingState("Identifier");
	
	allFrags.push_back(nfaForIDs);


	

	//NFAFragment nfaForIF		= NFAFragment::createNFAForString("if");
	//NFAFragment nfaForELSE		= NFAFragment::createNFAForString("else");
	//NFAFragment nfaForAND		= NFAFragment::createNFAForString("and");
	//NFAFragment nfaForOR		= NFAFragment::createNFAForString("or");
	//NFAFragment nfaForSTRUCT	= NFAFragment::createNFAForString("struct");
	//
	//NFAFragment nfaForKeyword =
	//	NFAFragment::makeParallel({ nfaForIF, nfaForELSE, nfaForAND, nfaForOR, nfaForSTRUCT })
	//	.addAcceptingState("Keyword");
	//
	//allFrags.push_back(nfaForKeyword);


	// for integer literals;
	// [+-]?[0-9]+
	NFAFragment nfaForIntegerLiteral =
		NFAFragment::createNFAForRange({ '+', '-' })
		.makeQuestionMark()
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9')
		.makePlus())
		.addAcceptingState("IntegerLiteral");

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
		.addAcceptingState("RealLiteral");

	allFrags.push_back(nfaForRealLiteral);


	// for operators;
	/*
		+
		-
		*
		/
		(
		)
	*/
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('+').addAcceptingState("Operator"));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('-').addAcceptingState("Operator"));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('*').addAcceptingState("Operator"));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('/').addAcceptingState("Operator"));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol('(').addAcceptingState("Operator"));
	allFrags.push_back(NFAFragment::createNFAForSingleSymbol(')').addAcceptingState("Operator"));

	//auto nfa = NFAFragment::makeParallel(allFrags);
	//cout << "NFA:\n" << nfa << "\n\n\n\n\n\n\n\n\n";

	dfa_ = NFAFragment::makeParallel(allFrags).convertToDFA();
}

Lexer::~Lexer()
{
}
