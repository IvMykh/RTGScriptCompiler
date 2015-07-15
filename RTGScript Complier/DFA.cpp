#include "DFA.h"
#include "Token.h"

#include <algorithm>
#include <sstream>

using namespace std;

DFA::DFA() :
	FiniteAutomaton(),
	transitions_()
{
}

DFA::DFA(DFA&& dfa) :
	FiniteAutomaton(dfa),
	transitions_(move(dfa.transitions_))
{
	//cout << "DFA::move-ctor\n";
}

bool DFA::acceptsString(const string& inputString) const
{
	string currStateName = this->initStateName_;
	MoveFunctionArgument arg;

	for each (auto inputSymbol in inputString)
	{
		arg.fromStateName_ = currStateName;
		arg.inputSymbol_ = inputSymbol;

		try
		{
			currStateName = this->transitions_.at(arg);
		}
		catch (out_of_range ex)
		{
			return false;
		}
	}

	auto iter = find_if(this->states_.begin(), this->states_.end(), [&currStateName](const State& st)
	{
		return st.name_ == currStateName;
	});

	if (iter != this->states_.end())
		return iter->isAccepting_;
	else
		throw exception("Current state is not present in state alphabet!");
}

vector<Token*> DFA::tokenize(const std::string& inputString) const
{
	vector<Token*> tokens;

	istringstream myStream(inputString);
	string currReadStr = "";

	while (myStream >> currReadStr)
	{
		const string* lastAcceptingStateName = nullptr;
		const string* currStateName = &this->initStateName_;
		int currTokenLength = 0;
		int charsAfterlastAccepting = 0;
		int startPos = 0;

		MoveFunctionArgument arg;
		for (int i = 0; i < currReadStr.length(); ++i)
		{
			arg.fromStateName_ = *currStateName;
			arg.inputSymbol_ = currReadStr[i];

			try
			{
				currStateName = &this->transitions_.at(arg);

				bool isCurrStateAccepting = any_of(this->states_.begin(), this->states_.end(),
					[&currStateName](const State& st)
				{
					return st.isAccepting_ && st.name_ == *currStateName;
				});

				if (isCurrStateAccepting)
				{
					lastAcceptingStateName = currStateName;
					charsAfterlastAccepting = 0;
				}
				else
				{
					++charsAfterlastAccepting;
				}

				++currTokenLength;
			}
			catch (out_of_range ex)
			{
				if (lastAcceptingStateName == nullptr)
				{
					string message = "Lexical error: word \"" + currReadStr.substr(startPos, currReadStr.length()) + "\"";
					throw exception(message.c_str());
				}

				int realTokenLen = currTokenLength - charsAfterlastAccepting;
				tokens.push_back(Token::createToken(this->stateNameAcceptingTokens_.at(*lastAcceptingStateName), currReadStr.substr(startPos, realTokenLen)));
				
				startPos = startPos + realTokenLen;
				currTokenLength = 0;

				i -= (charsAfterlastAccepting + 1);

				lastAcceptingStateName = nullptr;
				currStateName = &this->initStateName_;
			}

			if (i == (currReadStr.length() - 1))
			{
				if (lastAcceptingStateName == nullptr)
				{
					string message = "Lexical error: word \"" + currReadStr.substr(startPos, currReadStr.length()) + "\"";
					throw exception(message.c_str());
				}

				int realTokenLen = currTokenLength - charsAfterlastAccepting;
				tokens.push_back(Token::createToken(this->stateNameAcceptingTokens_.at(*lastAcceptingStateName), currReadStr.substr(startPos, realTokenLen)));

				break;
			}
		}
	}

	return tokens;
}

DFA& DFA::operator=(DFA&& dfa)
{
	if (this != &dfa)
	{
		this->FiniteAutomaton::operator=(dfa);
		this->transitions_ = move(dfa.transitions_);

		//cout << "DFA::move-assign\n";
	}

	return *this;
}

ostream& operator<<(ostream& os, const DFA& dfa)
{
	os << *(FiniteAutomaton*)(&dfa);

	os << "   Transitions: \n";
	for each (auto elem in dfa.transitions_)
	{
		os << elem.first.fromStateName_ << ' ' << elem.first.inputSymbol_ << "  ->  " << elem.second;
		os << '\n';
	}

	return os;
}

istream& operator>>(istream& is, DFA& dfa)
{
	is >> *(FiniteAutomaton*)(&dfa);

	string line = "";
	string titleString = "";
	string stateName = "";

	// reading transitions;
	map <MoveFunctionArgument, string> transitions;

	is >> titleString;
	getline(is, line);

	istringstream myStream;

	while (getline(is, line))
	{
		myStream.clear();
		myStream.str(line);
		myStream.seekg(0, is.beg);

		string fromStateName = "";
		myStream >> fromStateName;

		char inputSymbol = '\0';
		myStream >> inputSymbol;

		myStream >> titleString;

		string toStateName;
		myStream >> toStateName;

		transitions[MoveFunctionArgument(fromStateName, inputSymbol)] = toStateName;
	}

	dfa.transitions_ = transitions;

	return is;
}