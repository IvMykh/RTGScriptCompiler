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
	// TODO: Tokenization;

	// testing result dfa;
	clock_t tStart = clock();
	Lexer l;
	cout << "Execution time is " << (double)(clock() - tStart) / CLOCKS_PER_SEC << '\n';
	
	ofstream ofstr("RESULTDFA.txt");
	ofstr << l.dfa_ << '\n';
	
	string input = "if( if17 + 10 / -.5 === 10!=	5<-74.)";
	
	try
	{
		vector<Token*> tokens = l.dfa_.tokenize(input);
	
		for_each(tokens.begin(), tokens.end(),
			[](const Token* token)
		{
			cout << token->getTokenTypeString() << " - ";
		
			switch (token->getTokenType())
			{
			case Token::Type::Keyword: cout << ((KeywordToken*)token)->getValue() << '\n'; break;
			case Token::Type::Identifier: cout << ((IdentifierToken*)token)->getValue() << '\n'; break;
			case Token::Type::IntegerLiteral: cout << ((IntegerLiteralToken*)token)->getValue() << '\n'; break;
			case Token::Type::RealLiteral: cout << ((RealLiteralToken*)token)->getValue() << '\n'; break;
			case Token::Type::Operator: cout << ((OperatorToken*)token)->getValue() << '\n'; break;
			default: cout << "NONE\n";
				break;
			}
		
			delete token;
		});

		cout << "\n\nShould accept:\n";
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("if") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("else") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("and") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("or") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("struct") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("*") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("/") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("(") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString(")") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("_12s13sd_8") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("15") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+15") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-15") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("10.09") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-10.09") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString(".2") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-.2") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+.2") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("12.") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+12.") << '\n';
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-12.") << '\n';

		cout << "\n\nShould not accept:\n";		
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("&if") << '\n'; // not accepts - it is good;
		cout << "Accepts? " << boolalpha << l.dfa_.acceptsString(".") << '\n';;
	}
	catch (exception& ex)
	{
		cout << ex.what() << '\n';
	}

	system("pause");
}