#pragma once
#include <vector>

#include "DFA.h"
#include "NFA.h"

class Lexer
{
private:
	DFA dfa_;

public:
	Lexer();
	Lexer(const std::string& fileName);

	std::vector<Token*> tokenize(const std::string& inputString) const;

	~Lexer();
};

