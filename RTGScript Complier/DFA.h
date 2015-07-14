#pragma once
#include "FiniteAutomaton.h"

class DFA
	:public FiniteAutomaton
{
	friend class NFAFragment;

	std::map<MoveFunctionArgument, std::string> transitions_;

public:
	DFA();
	DFA(DFA&& dfa);

	bool acceptsString(const std::string& inputString) const;

	DFA& operator=(DFA&& dfa);

	friend std::ostream& operator<<(std::ostream& os, const DFA& dfa);
	friend std::istream& operator>>(std::istream& is, DFA& dfa);
};

