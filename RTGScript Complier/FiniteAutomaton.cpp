#include "FiniteAutomaton.h"

#include <fstream>
#include <sstream>

#include <algorithm>

using namespace std;

FiniteAutomaton::FiniteAutomaton() :
	states_(),
	initStateName_(),
	stateNameAcceptingTokens_()
{
}

FiniteAutomaton::FiniteAutomaton(const FiniteAutomaton& fa):
	states_(fa.states_),
	initStateName_(fa.initStateName_),
	stateNameAcceptingTokens_(fa.stateNameAcceptingTokens_)
{
}

FiniteAutomaton::FiniteAutomaton(FiniteAutomaton&& fa) :
	states_(move(fa.states_)),
	initStateName_(move(fa.initStateName_)),
	stateNameAcceptingTokens_(move(fa.stateNameAcceptingTokens_))
{
	//cout << "FiniteAutomaton::move-ctor\n";
}

bool FiniteAutomaton::isStateAccepting(const std::string& stateName) const
{
	return (find_if(this->states_.begin(), this->states_.end(),
		[&stateName](const State& st)
	{
		return st.isAccepting_ && st.name_ == stateName;
	}) != this->states_.end());
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
		this->stateNameAcceptingTokens_ = move(fa.stateNameAcceptingTokens_);

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
		this->stateNameAcceptingTokens_ = fa.stateNameAcceptingTokens_;
	}
	return *this;
}

void FiniteAutomaton::serialize(ostream& os) const
{
	/* File format:
		first line:		non-accepting state names;
		second line:	accepting state names (<state name> <accepting token value>);
		third line:		initial state name;
		
		further lines: transitions (<from-state name> <input symbol> <to-state name>);
	*/

	for_each(this->states_.begin(), this->states_.end(),
		[&os](const State& state)
	{
		if (!state.isAccepting_)
		{
			os << state.name_ << ' ';
		}
	});
	os << '\n';

	for_each(this->states_.begin(), this->states_.end(),
		[&os, this](const State& state)
	{
		if (state.isAccepting_)
		{
			os << state.name_ << ' ' << (int)this->stateNameAcceptingTokens_.at(state.name_) << '\t';
		}
	});
	os << '\n';

	os << this->initStateName_ << '\n';
}

void FiniteAutomaton::deserialize(istream& is)
{
	/* File format:
	first line:		non-accepting state names;
	second line:	accepting state names (<state name> <accepting token value>);
	third line:		initial state name;

	further lines: transitions (<from-state name> <input symbol> <to-state name>);
	*/

	this->states_.clear();
	this->initStateName_.clear();
	this->stateNameAcceptingTokens_.clear();

	string line = "";
	getline(is, line);

	// reading non-accepting state names;
	istringstream myStream(line);
	copy(istream_iterator<string>(myStream), istream_iterator<string>(), inserter(this->states_, this->states_.begin()));

	getline(is, line);

	myStream.clear();
	myStream.str(line);

	// reading accepting state names;
	string stateName = "";
	int tokenTypeValue = 0;

	while (!myStream.eof())
	{
		myStream >> stateName >> tokenTypeValue;

		this->states_.insert(State(stateName, true));
		this->stateNameAcceptingTokens_[stateName] = (Token::Type)tokenTypeValue;
	}

	// reading initial state name;
	is >> this->initStateName_;
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