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

	// testing result dfa;
	clock_t tStart = clock();
	Lexer l;
	cout << "Execution time is " << (double)(clock() - tStart) / CLOCKS_PER_SEC << '\n';
	cout << l.dfa_ << '\n';
	
	cout << boolalpha << l.dfa_.acceptsString("if") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("else") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("and") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("or") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("struct") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("+") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("-") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("*") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("/") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("(") << '\n';
	cout << boolalpha << l.dfa_.acceptsString(")") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("_12s13sd_8") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("15") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("+15") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("-15") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("10.09") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("-10.09") << '\n';
	cout << boolalpha << l.dfa_.acceptsString(".2") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("-.2") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("+.2") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("12.") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("+12.") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("-12.") << '\n';
	cout << boolalpha << l.dfa_.acceptsString("&") << '\n'; // not accepts - it is good;
	
	
	ofstream file("RESULTDFA.txt");
	file << l.dfa_ << '\n';

	system("pause");
}