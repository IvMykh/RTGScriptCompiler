#pragma once

#include <set>
#include <map>

#include "UtilityClasses.h"

class FiniteAutomaton
{
protected:
	// convention: # - epsilon;
	std::set<State> states_;
	std::string initStateName_;

public:
	FiniteAutomaton();
	FiniteAutomaton(const FiniteAutomaton& fa);
	FiniteAutomaton(FiniteAutomaton&& fa);

	virtual ~FiniteAutomaton() = 0;

	FiniteAutomaton& operator=(FiniteAutomaton&& fa);
	FiniteAutomaton& operator=(const FiniteAutomaton& fa);

	friend std::istream& operator>>(std::istream& is, FiniteAutomaton& fa);
	friend std::ostream& operator<<(std::ostream& os, const FiniteAutomaton& fa);
};

