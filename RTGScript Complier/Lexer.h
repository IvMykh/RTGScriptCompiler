#pragma once
#include <vector>

#include "DFA.h"
#include "NFA.h"

class Lexer
{
public:
	DFA dfa_;

public:
	Lexer();
	~Lexer();
};

