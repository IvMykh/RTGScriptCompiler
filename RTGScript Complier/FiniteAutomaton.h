#pragma once

#include <set>
#include <map>

#include "UtilityClasses.h"
#include "Token.h"

class FiniteAutomaton
{
protected:
	// convention: # - epsilon;
	std::set<State> states_;
	std::string initStateName_;

	std::map<std::string, Token::Type> stateNameAcceptingTokens_;

public:
	FiniteAutomaton();
	FiniteAutomaton(const FiniteAutomaton& fa);
	FiniteAutomaton(FiniteAutomaton&& fa);

	bool isStateAccepting(const std::string& stateName) const;

	virtual ~FiniteAutomaton() = 0;

	FiniteAutomaton& operator=(FiniteAutomaton&& fa);
	FiniteAutomaton& operator=(const FiniteAutomaton& fa);


	virtual void serialize(std::ostream& os) const;
	virtual void deserialize(std::istream& is);

	friend std::istream& operator>>(std::istream& is, FiniteAutomaton& fa);
	friend std::ostream& operator<<(std::ostream& os, const FiniteAutomaton& fa);
};

