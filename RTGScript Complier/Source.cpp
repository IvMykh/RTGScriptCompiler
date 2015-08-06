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

#include "Parser.h"
#include "TreeSerializer.h"

using namespace std;



void  main()
{
	// testing result dfa;

	string serDFAFileName = "Serialized result DFA.txt"; // name not to be changed;
	
	clock_t tStart = clock();
	Lexer l(serDFAFileName);
	cout << "Execution time is " << (double)(clock() - tStart) / CLOCKS_PER_SEC << '\n';
	
	//ofstream ofstr("RESULTDFA.txt");
	//ofstr << l.dfa_ << '\n';
	//string input = "if( if17 + 10 / -.5 === 10!=	5<-74.)";
	//	
	//cout << "\n\nShould accept:\n";
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("if") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("else") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("and") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("or") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("struct") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("*") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("/") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("(") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString(")") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("_12s13sd_8") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("15") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+15") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-15") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("10.09") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-10.09") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString(".2") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-.2") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+.2") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("12.") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("+12.") << '\n';
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("-12.") << '\n';
	//
	//cout << "\n\nShould not accept:\n";		
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString("&if") << '\n'; // not accepts - it is good;
	//cout << "Accepts? " << boolalpha << l.dfa_.acceptsString(".") << '\n';;
	
	string userCodeFileName = "UserCode3.txt";
	ifstream userCodeFile(userCodeFileName);

	if (!userCodeFile)
	{
		cout << "File \"" << userCodeFileName << "\n not found\n";
		return;
	}

	string userCode = "";
	userCode.assign(istreambuf_iterator<char>(userCodeFile), istreambuf_iterator<char>());

	userCodeFile.close();

	string lexerOutputFileName = "Lexer output.txt";
	ofstream lexerOutputFile(lexerOutputFileName);

	try
	{
		vector<Token*> tokens = l.tokenize(userCode);
	
		for_each(tokens.begin(), tokens.end(),
			[&lexerOutputFile](const Token* token)
		{
			lexerOutputFile << token->getTokenTypeString() << "\t\t\t- \t\t\t";
		
			switch (token->getTokenType())
			{
			case Token::Type::Keyword:			lexerOutputFile << ((KeywordToken*)token)->getValue() << '\n'; break;
			case Token::Type::Identifier:		lexerOutputFile << ((IdentifierToken*)token)->getValue() << '\n'; break;
			case Token::Type::IntegerLiteral:	lexerOutputFile << ((IntegerLiteralToken*)token)->getValue() << '\n'; break;
			case Token::Type::RealLiteral:		lexerOutputFile << ((RealLiteralToken*)token)->getValue() << '\n'; break;
			case Token::Type::Operator:			lexerOutputFile << ((OperatorToken*)token)->getValue() << '\n'; break;
			default:							throw exception("Unexpected token type\n");
				break;
			}
		
			//delete token;
		});
		
		Parser parser(tokens);
		try
		{
			auto prog = parser.parse();
			prog->Accept(new TreeSerializer(parser.getStorages(), cout));
		}
		catch (ParserError pe)
		{
			cout << "[ParserError] " << pe.what() << '\n';
		}

	}
	catch (exception& ex)
	{
		cout << ex.what() << '\n';
	}

	system("pause");
}