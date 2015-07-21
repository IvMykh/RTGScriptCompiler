#include "DFA.h"
#include "Token.h"

#include <sstream>
#include <fstream>

#include <algorithm>

using namespace std;



// utility functions;
const string digitToChar(int digit)
{
	switch (digit)
	{
	case 0: return "0";
	case 1: return "1";
	case 2: return "2";
	case 3: return "3";
	case 4: return "4";
	case 5: return "5";
	case 6: return "6";
	case 7: return "7";
	case 8: return "8";
	case 9: return "9";

	default: throw exception("Error! Integer is not a digit");
	}
}

const string intToString(int number)
{
	string resStr = "";

	while (number != 0)
	{
		resStr = digitToChar(number % 10) + resStr;
		number /= 10;
	}

	if (resStr == "")
		return "0";

	return resStr;
}


// class LexicalErrorException definition;
LexicalErrorException::LexicalErrorException(const char* msg) :
	exception(msg)
{
}

// class DFA definition;
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
	
	istringstream allCodeStream(inputString);
	string lineOfCode = "";

	for (int lineNumber = 1; getline(allCodeStream, lineOfCode, '\n'); ++lineNumber)
	{
		istringstream lineOfCodeStream(lineOfCode);
		string currReadStr = "";

		while (lineOfCodeStream >> currReadStr)
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
						string message = "LINE " + intToString(lineNumber) + " \t-\t " 
							+ " Lexical error: word \"" + currReadStr.substr(startPos, currReadStr.length()) + "\"";
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
						string message = "LINE " + intToString(lineNumber) + " \t-\t "
							+ " Lexical error: word \"" + currReadStr.substr(startPos, currReadStr.length()) + "\"";
						throw LexicalErrorException(message.c_str());
					}

					int realTokenLen = currTokenLength - charsAfterlastAccepting;
					tokens.push_back(Token::createToken(this->stateNameAcceptingTokens_.at(*lastAcceptingStateName), currReadStr.substr(startPos, realTokenLen)));

					break;
				}
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



void DFA::serialize(ostream& os) const
{
	/* File format:
	first line:		non-accepting state names;
	second line:	accepting state names (<state name> <accepting token value>);
	third line:		initial state name;

	further lines: transitions (<from-state name> <input symbol> <to-state name>);
	*/

	FiniteAutomaton::serialize(os);

	for (auto transition : this->transitions_)
	{
		os << transition.first.fromStateName_ << ' ' << transition.first.inputSymbol_ << ' ' << transition.second << '\n';
	}
}

void DFA::deserialize(istream& is)
{
	/* File format:
	first line:		non-accepting state names;
	second line:	accepting state names (<state name> <accepting token value>);
	third line:		initial state name;

	further lines: transitions (<from-state name> <input symbol> <to-state name>);
	*/

	this->transitions_.clear();

	FiniteAutomaton::deserialize(is);

	// reading transitions;
	string fromStateName = "";
	char inputSymbool = '\0';
	string toStateName = "";
	
	while (!is.eof())
	{
		is >> fromStateName >> inputSymbool >> toStateName;
		this->transitions_[MoveFunctionArgument(fromStateName, inputSymbool)] = toStateName;
	}
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