#pragma once
#include "FiniteAutomaton.h"

#include <vector>

class Token;

class LexicalErrorException
	:public std::exception
{
public:
	LexicalErrorException(const char* msg);
};

class DFA
	:public FiniteAutomaton
{
	friend class NFAFragment;

	std::map<MoveFunctionArgument, std::string> transitions_;

public:
	DFA();
	DFA(DFA&& dfa);

	bool acceptsString(const std::string& inputString) const;

	std::vector<Token*> tokenize(const std::string& inputString) const;

	DFA& operator=(DFA&& dfa);

	virtual void serialize(std::ostream& os) const override;
	virtual void deserialize(std::istream& is) override;

	friend std::ostream& operator<<(std::ostream& os, const DFA& dfa);
	friend std::istream& operator>>(std::istream& is, DFA& dfa);
};

