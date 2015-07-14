#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#include <string>
#include <algorithm>

#include "DFA.h"
#include "NFA.h"
#include "Lexer.h"
#include "Token.h"

using namespace std;



void  main()
{
	// TODO: Token types and lexer;
	
	//vector<int> v;
	//fill_n(back_inserter(v), 10000000, 5);
	//
	//clock_t tStart = clock();
	//vector<int> copyv;
	//copyv = move(v);
	//cout << "Execution time is " << (double)(clock() - tStart) / CLOCKS_PER_SEC;
	//
	////v[0] = 2;
	//copyv[0] = 2;



	int a;


	// testing result dfa;
	//clock_t tStart = clock();
	//Lexer l;
	//cout << "Execution time is " << (double)(clock() - tStart) / CLOCKS_PER_SEC << '\n';
	//cout << l.dfa_ << '\n';
	//
	//cout << boolalpha << l.dfa_.acceptsString("if") << '\n';
	//cout << boolalpha << l.dfa_.acceptsString("else") << '\n';
	//cout << boolalpha << l.dfa_.acceptsString("+") << '\n';
	//cout << boolalpha << l.dfa_.acceptsString("(") << '\n';
	//cout << boolalpha << l.dfa_.acceptsString("_12s13sd_8") << '\n';
	//cout << boolalpha << l.dfa_.acceptsString("0.9") << '\n';
	//cout << boolalpha << l.dfa_.acceptsString("+15") << '\n';
	//cout << boolalpha << l.dfa_.acceptsString("-3") << '\n';

	//ofstream file("RESULTDFA.txt");
	//file << l.dfa_ << '\n';
	//
	//cout << boolalpha << l.dfa_.acceptsString("if");
	//cout << boolalpha << l.dfa_.acceptsString("else");
	//cout << boolalpha << l.dfa_.acceptsString("+");
	//cout << boolalpha << l.dfa_.acceptsString("(");
	//cout << boolalpha << l.dfa_.acceptsString("_12s13sd_8");
	//cout << boolalpha << l.dfa_.acceptsString("-.9");
	//cout << boolalpha << l.dfa_.acceptsString("15");
	//cout << boolalpha << l.dfa_.acceptsString("-3");
	//
	//cout << '\n' << boolalpha << l.dfa_.acceptsString("&");

	//float a = -2.;


	// for identifiers;
	// [_a-zA-Z][_a-zA-z0-9]*

	//NFAFragment nfaForaToz = NFAFragment::createNFAForRange('a', 'z');
	//NFAFragment nfaForAToZ = NFAFragment::createNFAForRange('A', 'Z');
	//NFAFragment nfaFor0To9 = NFAFragment::createNFAForRange('0', '9');
	//NFAFragment nfaForUnderscore = NFAFragment::createNFAForSingleSymbol('_');
	//
	//NFAFragment nfaForIDs =
	//	NFAFragment::makeParallel({ nfaForaToz, nfaForAToZ, nfaForUnderscore })
	//	.makeConcatenation(
	//	NFAFragment::makeParallel({ nfaForaToz, nfaForAToZ, nfaForUnderscore, nfaFor0To9 }).makeStar()
	//	)
	//	.addAcceptingState("Identifier");
	//
	//clock_t tStart = clock();
	//nfaForIDs.convertToDFA();
	//cout << "Execution time is " << (double)(clock() - tStart) / CLOCKS_PER_SEC;


	//auto nfa = NFAFragment::createNFAForString("else").addAcceptingState("Keyword");
	//auto dfa = nfa.convertToDFA();
	//cout << dfa << '\n';

	
	// nfa for real literal;
	// [+-]?([0-9]+.[0-9]*|.[0-9]+)
	NFAFragment nfaForPlusMinus =
		(NFAFragment::createNFAForRange({ '+', '-' })
		.makeQuestionMark());
	
	NFAFragment nfaForfirstForm =
		(NFAFragment::createNFAForRange('0', '9')
		.makePlus()
		.makeConcatenation(NFAFragment::createNFAForSingleSymbol('.'))
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makeStar()));
	
	NFAFragment nfaForSecondForm =
		(NFAFragment::createNFAForSingleSymbol('.')
		.makeConcatenation(NFAFragment::createNFAForRange('0', '9').makePlus()));
	
	auto nfa =
		(nfaForPlusMinus
		.makeConcatenation(NFAFragment::makeParallel({ nfaForfirstForm, nfaForSecondForm }))
		.addAcceptingState("RealLiteral"));
	
		auto dfa = nfa.convertToDFA();

	//cout << boolalpha << dfa.acceptsString("-.9") << '\n';

	system("pause");
}