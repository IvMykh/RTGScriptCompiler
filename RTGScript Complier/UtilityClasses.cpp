#include "UtilityClasses.h"

using namespace std;

//struct State definition;
State::State(const string& name, const bool isAccepting) :
	name_(name),
	isAccepting_(isAccepting)
{
}

State::State(State&& st) :
	name_(move(st.name_)),
	isAccepting_(st.isAccepting_)
{
	//cout << "State::move-ctor\n";
}

State& State::operator=(const State& st)
{
	if (&st != this)
	{
		this->name_ = st.name_;
		this->isAccepting_ = st.isAccepting_;
	}

	return *this;
}

State& State::operator=(State&& st)
{
	if (&st != this)
	{
		this->name_ = move(st.name_);
		this->isAccepting_ = st.isAccepting_;

		//cout << "State::move-assign\n";
	}
	return *this;
}

bool State::operator<(const State& other) const
{
	return this->name_ < other.name_;
}

ostream& operator<<(ostream& os, const State& st)
{
	return os << st.name_ << "\t is accepting - " << boolalpha << st.isAccepting_;
}

//struct MoveFunctionArgument definition;
MoveFunctionArgument::MoveFunctionArgument(const string& fromStateName, const char inputSymbol) :
	fromStateName_(fromStateName),
	inputSymbol_(inputSymbol)
{
}

MoveFunctionArgument::MoveFunctionArgument(MoveFunctionArgument&& mfa) :
	fromStateName_(move(mfa.fromStateName_)),
	inputSymbol_(mfa.inputSymbol_)
{
	//cout << "MoveFunctionArgument::move-ctor\n";
}


MoveFunctionArgument& MoveFunctionArgument::operator = (MoveFunctionArgument&& mfa)
{
	if (this != &mfa)
	{
		this->fromStateName_ = move(mfa.fromStateName_);
		this->inputSymbol_ = move(mfa.inputSymbol_);

		//cout << "MoveFunctionArgument::move-assign\n";
	}

	return *this;
}

bool MoveFunctionArgument::operator<(const MoveFunctionArgument& mfa) const
{
	bool areNotEqual = this->fromStateName_ != mfa.fromStateName_;

	if (areNotEqual)
		return this->fromStateName_ < mfa.fromStateName_;
	else
		return this->inputSymbol_ < mfa.inputSymbol_;

}



//struct IncomingHalfTransition definition;
IncomingHalfTransition::IncomingHalfTransition(const string& toStateName) :
	toStateName_(toStateName)
{
}


IncomingHalfTransition::IncomingHalfTransition(IncomingHalfTransition&& iht):
	toStateName_(move(iht.toStateName_))
{
	//cout << "IncomingHalfTransition::move-ctor\n";
}

IncomingHalfTransition& IncomingHalfTransition::operator = (IncomingHalfTransition&& iht)
{
	if (this != &iht)
	{
		this->toStateName_ = move(iht.toStateName_);

		//cout << "IncomingHalfTransition::move-assing\n";
	}

	return *this;
}


// struct OutcomingHalfTransition definition;
OutcomingHalfTransition::OutcomingHalfTransition(const string& fromStateName, const char inputSymbol) :
	fromStateName_(fromStateName),
	inputSymbol_(inputSymbol)
{
}

OutcomingHalfTransition::OutcomingHalfTransition(OutcomingHalfTransition&& oht) :
	fromStateName_(move(oht.fromStateName_)),
	inputSymbol_(oht.inputSymbol_)
{
	//cout << "OutcomingHalfTransition::move-ctor\n";
}

OutcomingHalfTransition& OutcomingHalfTransition::operator = (OutcomingHalfTransition&& oht)
{
	if (this != &oht)
	{
		this->fromStateName_ = move(oht.fromStateName_);
		this->inputSymbol_ = oht.inputSymbol_;

		//cout << "OutcomingHalfTransition::move-assing\n";
	}

	return *this;
}