#include "FiniteAutomaton.h"

#include <sstream>

using namespace std;

FiniteAutomaton::FiniteAutomaton() :
	states_(),
	initStateName_()
{
}

FiniteAutomaton::FiniteAutomaton(const FiniteAutomaton& fa):
	states_(fa.states_),
	initStateName_(fa.initStateName_)
{
}

FiniteAutomaton::FiniteAutomaton(FiniteAutomaton&& fa) :
	states_(move(fa.states_)),
	initStateName_(move(fa.initStateName_))
{
	//cout << "FiniteAutomaton::move-ctor\n";
}

FiniteAutomaton::~FiniteAutomaton()
{
}

FiniteAutomaton& FiniteAutomaton::operator = (FiniteAutomaton&& fa)
{
	if (this != &fa)
	{
		this->states_ = move(fa.states_);
		this->initStateName_ = move(fa.initStateName_);

		//cout << "FiniteAutomaton::move-assign\n";
	}
	return *this;
}

FiniteAutomaton& FiniteAutomaton::operator = (const FiniteAutomaton& fa)
{
	if (this != &fa)
	{
		this->states_ = fa.states_;
		this->initStateName_ = fa.initStateName_;
	}
	return *this;
}

istream& operator>>(istream& is, FiniteAutomaton& fa)
{
	string line = "";
	string titleString = "";

	// reading non-accepting states;
	getline(is, line);
	istringstream myStream(line);

	myStream >> titleString;

	string stateName = "";
	while (myStream >> stateName)
	{
		fa.states_.insert(State(stateName));
	}

	// reading accepting states;
	getline(is, line);
	myStream.clear();
	myStream.str(line);
	myStream.seekg(0, is.beg);

	myStream >> titleString;

	while (myStream >> stateName)
	{
		fa.states_.insert(State(stateName, true));
	}

	// reading initial state;
	is >> titleString;
	is >> stateName;
	fa.initStateName_ = stateName;

	return is;
}

ostream& operator<<(std::ostream& os, const FiniteAutomaton& fa)
{
	os << "NFA Fragment:" << '\n';

	os << "   States: \n";
	copy(fa.states_.begin(), fa.states_.end(), ostream_iterator<State>(os, "\n"));

	os << "   Initial state name: " << fa.initStateName_ << '\n';

	return os;
}