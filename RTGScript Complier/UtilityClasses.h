#pragma once
#include <iostream>
#include <string>



// State of NFA;
struct State
{
	std::string name_;
	bool isAccepting_;

	State(const std::string& name = "", const bool isAccepting = false);
	State(State&& st);

	bool operator<(const State& other) const;
	
	State& operator=(const State& st);
	State& operator=(State&& st);

	friend std::ostream& operator<<(std::ostream& os, const State& st);
};



// Key in map of transitions;
struct MoveFunctionArgument
{
	std::string fromStateName_;
	char inputSymbol_;

	MoveFunctionArgument(const std::string& fromStateName = "", const char inputSymbol = '\0');
	MoveFunctionArgument(MoveFunctionArgument&& mfa);


	MoveFunctionArgument& operator=(MoveFunctionArgument&& mfa);

	bool operator<(const MoveFunctionArgument& mfa) const;
};



// Half-transition structs;
struct IncomingHalfTransition
{
	std::string toStateName_;

	IncomingHalfTransition(const std::string& toStateName = "");
	IncomingHalfTransition(IncomingHalfTransition&& iht);

	IncomingHalfTransition& operator=(IncomingHalfTransition&& iht);
};



struct OutcomingHalfTransition
{
	std::string fromStateName_;
	char inputSymbol_;

	OutcomingHalfTransition(const std::string& fromStateName = "", const char inputSymbol = '\0');
	OutcomingHalfTransition(OutcomingHalfTransition&& oht);

	OutcomingHalfTransition& operator=(OutcomingHalfTransition&& oht);
};