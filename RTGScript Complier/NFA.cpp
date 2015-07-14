#include "NFA.h"

#include <sstream>
#include <algorithm>
#include <vector>

#include "Token.h"

using namespace std;

// utility functions;
bool isCharDigit(const char ch)
{
	return 48 <= ch && ch <= 57;
}

bool isStringInteger(const string& s)
{
	auto it = s.begin();
	while (it != s.end() && isCharDigit(*it)) 
		++it;

	return !s.empty() && it == s.end();
}

// class NFAStateNamesGenerator definition;
const string StateNamesGenerator::digitToChar(int digit)
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
	default: return "\0";
	}
}

const string StateNamesGenerator::intToString(int i)
{
	string resStr = "";

	while (i != 0)
	{
		resStr = digitToChar(i % 10) + resStr;
		i /= 10;
	}

	if (resStr == "")
		return "0";

	return resStr;
}

StateNamesGenerator::StateNamesGenerator() :
	counter_(0)
{
}

const string StateNamesGenerator::getNextAvailableName()
{
	return intToString(counter_++);
}



// struct MarkableStateName definition;
MarkableStateName::MarkableStateName(const string& stateName, const bool isMarked) :
	stateName_(stateName),
	isMarked_(isMarked)
{
}

bool MarkableStateName::operator<(const MarkableStateName& msn) const
{
	return this->stateName_ < msn.stateName_;
}



// class NFA definition;
StateNamesGenerator NFAFragment::nfaStateNamesGen_ = StateNamesGenerator();
StateNamesGenerator NFAFragment::dfaStateNamesGen_ = StateNamesGenerator();

// primitives creation functions;
NFAFragment NFAFragment::createNFAForEpsilon()
{
	return NFAFragment::createNFAForSingleSymbol('#');
}

NFAFragment NFAFragment::createNFAForSingleSymbol(const char symbol)
{
	NFAFragment resFrag;

	string stateName = nfaStateNamesGen_.getNextAvailableName();

	resFrag.states_.insert(State(stateName));
	resFrag.initStateName_ = stateName;

	resFrag.incomingHalfTrans_ = IncomingHalfTransition(stateName);
	resFrag.outcomingHalfTrans_ = OutcomingHalfTransition(stateName, symbol);

	return resFrag;
}

NFAFragment NFAFragment::createNFAForString(const string& str)
{
	vector <NFAFragment> singleSymbolFrags;
	for_each(str.begin(), str.end(), [&singleSymbolFrags](const char symbol)
	{
		singleSymbolFrags.push_back(NFAFragment::createNFAForSingleSymbol(symbol));
	});

	auto beginIter = singleSymbolFrags.begin();

	NFAFragment result = *(beginIter++);

	for_each(beginIter, singleSymbolFrags.end(), [&result](const NFAFragment& frag)
	{
		result.makeConcatenation(frag);
	});
	
	//return NFAFragment::makeConcatenation(singleSymbolFrags);

	return result;
}

NFAFragment NFAFragment::createNFAForRange(const char first, const char last)
{
	NFAFragment ending = NFAFragment::createNFAForEpsilon();
	NFAFragment result;

	result.states_.insert(nfaStateNamesGen_.getNextAvailableName());
	result.states_.insert(nfaStateNamesGen_.getNextAvailableName());

	const State* firstState = &*result.states_.begin();
	const State* secondState = &*result.states_.rbegin();

	result.initStateName_ = firstState->name_;

	for (int i = first; i <= last; ++i)
	{
		result.transitions_[MoveFunctionArgument(firstState->name_, i)]
			.insert(secondState->name_);
	}

	result.incomingHalfTrans_.toStateName_ = firstState->name_;
	
	result.outcomingHalfTrans_.fromStateName_ = secondState->name_;
	result.outcomingHalfTrans_.inputSymbol_ = '#';
	
	return result;
}

NFAFragment NFAFragment::createNFAForRange(const std::vector<char>& symbols)
{
	NFAFragment ending = NFAFragment::createNFAForEpsilon();
	NFAFragment result;

	result.states_.insert(nfaStateNamesGen_.getNextAvailableName());
	result.states_.insert(nfaStateNamesGen_.getNextAvailableName());

	const State* firstState = &*result.states_.begin();
	const State* secondState = &*result.states_.rbegin();

	result.initStateName_ = firstState->name_;

	for_each(symbols.begin(), symbols.end(), [&result, &firstState, &secondState](const char symb)
	{
		result.transitions_[MoveFunctionArgument(firstState->name_, symb)]
			.insert(secondState->name_);
	});

	result.incomingHalfTrans_.toStateName_ = firstState->name_;

	result.outcomingHalfTrans_.fromStateName_ = secondState->name_;
	result.outcomingHalfTrans_.inputSymbol_ = '#';

	return result;
}



// compositional functions;
//NFAFragment NFAFragment::makeConcatenation(const vector<NFAFragment>& fragments)
//{
//	NFAFragment result;
//
//	if (fragments.empty())
//		return result;
//
//	for_each(fragments.begin(), fragments.end(), [&result](const NFAFragment& nfaFrag)
//	{
//		result.states_.insert(nfaFrag.states_.begin(), nfaFrag.states_.end());
//	});
//
//	result.initStateName_ = fragments.begin()->initStateName_;
//
//	for_each(fragments.begin(), fragments.end(), [&result](const NFAFragment& nfaFrag)
//	{
//		result.transitions_.insert(nfaFrag.transitions_.begin(), nfaFrag.transitions_.end());
//	});
//
//	for (int i = 0; i < fragments.size() - 1; i++)
//	{
//		result.transitions_[MoveFunctionArgument(fragments[i].outcomingHalfTrans_.fromStateName_, fragments[i].outcomingHalfTrans_.inputSymbol_)]
//			.insert(fragments[i + 1].incomingHalfTrans_.toStateName_);
//	}
//
//	result.incomingHalfTrans_ = fragments.front().incomingHalfTrans_;
//	result.outcomingHalfTrans_ = fragments.back().outcomingHalfTrans_;
//
//	return result;
//}
//
NFAFragment NFAFragment::makeParallel(vector<NFAFragment> fragments)
{
	if (fragments.empty())
		return NFAFragment();

	NFAFragment result = NFAFragment::createNFAForEpsilon();
	NFAFragment ending = NFAFragment::createNFAForEpsilon();

	for_each(fragments.begin(), fragments.end(), [&result, &ending](NFAFragment& frag)
	{
		// midle - end;
		frag.states_.insert(ending.states_.begin(), ending.states_.end());
		
		frag.transitions_.insert(ending.transitions_.begin(), ending.transitions_.end());
		frag.transitions_[MoveFunctionArgument(frag.outcomingHalfTrans_.fromStateName_, frag.outcomingHalfTrans_.inputSymbol_)]
			.insert(ending.incomingHalfTrans_.toStateName_);

		frag.outcomingHalfTrans_ = ending.outcomingHalfTrans_;
		
		// begin - midle;
		result.states_.insert(frag.states_.begin(), frag.states_.end());
		
		result.transitions_.insert(frag.transitions_.begin(), frag.transitions_.end());
		result.transitions_[MoveFunctionArgument(result.outcomingHalfTrans_.fromStateName_, result.outcomingHalfTrans_.inputSymbol_)]
			.insert(frag.incomingHalfTrans_.toStateName_);
	});

	result.outcomingHalfTrans_ = ending.outcomingHalfTrans_;

	return move(result);
}
//
//NFAFragment NFAFragment::makeStar(const NFAFragment& frag)
//{
//	NFAFragment result = NFAFragment::createNFAForEpsilon();
//
//	result.states_.insert(frag.states_.begin(), frag.states_.end());
//	result.transitions_.insert(frag.transitions_.begin(), frag.transitions_.end());
//
//	result.transitions_[MoveFunctionArgument(result.outcomingHalfTrans_.fromStateName_, result.outcomingHalfTrans_.inputSymbol_)]
//		.insert(frag.initStateName_);
//
//	result.transitions_[MoveFunctionArgument(frag.outcomingHalfTrans_.fromStateName_, frag.outcomingHalfTrans_.inputSymbol_)]
//		.insert(result.incomingHalfTrans_.toStateName_);
//
//	return result;
//}
//
//NFAFragment NFAFragment::makePlus(const NFAFragment& frag)
//{
//	
//	NFAFragment ending = NFAFragment::createNFAForEpsilon();
//	NFAFragment newFrag = NFAFragment::makeConcatenation({ frag, ending });
//	
//	NFAFragment result = NFAFragment::createNFAForEpsilon();
//	
//	result.states_.insert(newFrag.states_.begin(), newFrag.states_.end());
//	result.transitions_.insert(newFrag.transitions_.begin(), newFrag.transitions_.end());
//	
//	result.transitions_[MoveFunctionArgument(newFrag.outcomingHalfTrans_.fromStateName_, newFrag.outcomingHalfTrans_.inputSymbol_)]
//		.insert(result.incomingHalfTrans_.toStateName_);
//	
//	result.transitions_[MoveFunctionArgument(result.outcomingHalfTrans_.fromStateName_, result.outcomingHalfTrans_.inputSymbol_)]
//		.insert(newFrag.initStateName_);
//	
//	result.outcomingHalfTrans_ = newFrag.outcomingHalfTrans_;
//	
//	return result;
//}
//
//NFAFragment NFAFragment::addAcceptingState(const NFAFragment& frag, const string& stateName)
//{ 
//	State st(stateName, true);
//
//	NFAFragment result = frag;	
//	result.states_.insert(st);
//
//	result.transitions_[MoveFunctionArgument(result.outcomingHalfTrans_.fromStateName_, result.outcomingHalfTrans_.inputSymbol_)]
//		.insert(st.name_);
//
//	return result;
//}



const string NFAFragment::getEpsilonClosure(const string& stateName/*, DfaNfaStatesMap& dfaNfaStatesMap*/) const
{
	set<MarkableStateName> resSet;
	istringstream myStr(stateName);
	copy(istream_iterator<string>(myStr), istream_iterator<string>(), inserter(resSet, resSet.begin()));

	bool isStateAccepting = false;

	auto msnIter = find_if(resSet.begin(), resSet.end(), [](const MarkableStateName& m) { return m.isMarked_ == false; });
	while (msnIter != resSet.end())
	{
		try
		{
			//isStateAccepting = isStateAccepting || any_of(this->states_.begin(), this->states_.end(), [&msnIter](const State& st)
			//{
			//	return (st.name_ == msnIter->stateName_) && st.isAccepting_;
			//});

			const set<string>& aSet = this->transitions_.at(MoveFunctionArgument(msnIter->stateName_, '#'));
			resSet.insert(aSet.begin(), aSet.end());
		}
		catch (out_of_range&)
		{
		}

		msnIter._Ptr->_Myval.isMarked_ = true;
		msnIter = (find_if(resSet.begin(), resSet.end(), [](const MarkableStateName& m) { return m.isMarked_ == false; }));
	}


	string resStateName = "";
	for_each(resSet.begin(), resSet.end(), [&resStateName](const MarkableStateName& m)
	{
		resStateName += (' ' + m.stateName_);
	});

	return resStateName;
}

const std::string NFAFragment::getReachableThroughS(const std::string& stateName, const char S/*, DfaNfaStatesMap& dfaNfaStatesMap*/) const
{
	set<MarkableStateName> markableStates;
	istringstream myStr(stateName);
	copy(istream_iterator<string>(myStr), istream_iterator<string>(), inserter(markableStates, markableStates.begin()));

	string resString = "";

	auto msnIter = find_if(markableStates.begin(), markableStates.end(), [](const MarkableStateName& m) { return m.isMarked_ == false; });
	while (msnIter != markableStates.end())
	{
		try
		{
			const set<string>& aSet = this->transitions_.at(MoveFunctionArgument(msnIter->stateName_, S));
			for_each(aSet.begin(), aSet.end(), [&resString](const string& str)
			{
				resString += (' ' + str);
			});
		}
		catch (out_of_range&)
		{
		}

		msnIter._Ptr->_Myval.isMarked_ = true;
		msnIter = (find_if(markableStates.begin(), markableStates.end(), [](const MarkableStateName& m) { return m.isMarked_ == false; }));
	}

	return resString;
}

NFAFragment::NFAFragment() :
	FiniteAutomaton(),
	incomingHalfTrans_(),
	outcomingHalfTrans_(),
	transitions_()
{
}

NFAFragment::NFAFragment(NFAFragment&& frag) :
	FiniteAutomaton(frag),
	incomingHalfTrans_(move(frag.incomingHalfTrans_)),
	outcomingHalfTrans_(move(frag.outcomingHalfTrans_)),
	transitions_(move(frag.transitions_))
{
	//cout << "NFAFragment::move-ctor\n";
}

const DFA NFAFragment::convertToDFA() const
{
	DfaNfaStatesMap dfaNfaStatesMap;

	set<char> externAlpha;
	for (auto& elem : this->transitions_)
	{
		char symbol = elem.first.inputSymbol_;
		if (symbol != '#')
		{
			externAlpha.insert(symbol);
		}
	}

	DFA resultDfa;

	string initDfaStateName = this->getEpsilonClosure({ this->initStateName_ });

	istringstream myStr(initDfaStateName);
	string currStateName = "";
	string maxPriorityStateName = "";
	int maxPriority = 0;

	while (myStr >> currStateName)
	{
		int currPriority = 0;
		if (!isStringInteger(currStateName) && (currPriority = (int)Token::getTypeByString(currStateName)) > maxPriority)
		{
			maxPriorityStateName = currStateName;
			maxPriority = currPriority;
		}
	}

	string shortDfaStateName = "";
	if (maxPriority > 0)
	{
		shortDfaStateName = maxPriorityStateName;
		resultDfa.states_.insert(State(shortDfaStateName, true));
	}
	else
	{
		shortDfaStateName = dfaStateNamesGen_.getNextAvailableName();
		resultDfa.states_.insert(State(shortDfaStateName, false));
	}

	dfaNfaStatesMap[shortDfaStateName] = initDfaStateName;
	
	resultDfa.initStateName_ = shortDfaStateName;
	
	set<MarkableStateName> markableDfaStates;
	markableDfaStates.insert(shortDfaStateName);
	
	auto msn = find_if(markableDfaStates.begin(), markableDfaStates.end(), [](const MarkableStateName& m) { return m.isMarked_ == false; });

	while (msn != markableDfaStates.end())
	{
		for (const char& symbol : externAlpha)
		{
			string newDfaStateName = "";
			try
			{
				newDfaStateName = this->getEpsilonClosure(this->getReachableThroughS(dfaNfaStatesMap.at(msn->stateName_), symbol));
			}
			catch (exception ex)
			{
				cout << "HA-HA-HA-HA-HA\n";
			}

			if (newDfaStateName != "")
			{
				istringstream myNewStr(newDfaStateName);
				currStateName = "";
				maxPriorityStateName = "";
				maxPriority = 0;
				
				while (myNewStr >> currStateName)
				{
					int currPriority = 0;
					if (!isStringInteger(currStateName) && (currPriority = (int)Token::getTypeByString(currStateName)) > maxPriority)
					{
						maxPriorityStateName = currStateName;
						maxPriority = currPriority;
					}
				}
				
				if (maxPriority > 0)
				{
					shortDfaStateName = maxPriorityStateName;
					resultDfa.states_.insert(State(shortDfaStateName, true));
				}
				else
				{
					shortDfaStateName = dfaStateNamesGen_.getNextAvailableName();
					resultDfa.states_.insert(State(shortDfaStateName, false));
				}

				//shortDfaStateName = dfaStateNamesGen_.getNextAvailableName();
				dfaNfaStatesMap[shortDfaStateName] = newDfaStateName;

				markableDfaStates.insert(shortDfaStateName);
				//resultDfa.states_.insert(State(shortDfaStateName, newDfaState.isAccepting_));
				resultDfa.transitions_[MoveFunctionArgument(msn->stateName_, symbol)] = shortDfaStateName;
			}
		}

		msn._Ptr->_Myval.isMarked_ = true;
		msn = find_if(markableDfaStates.begin(), markableDfaStates.end(), [](const MarkableStateName& m) { return m.isMarked_ == false; });
	}

	return resultDfa; //~
}



// compositional modifying methods
NFAFragment& NFAFragment::makeConcatenation(const NFAFragment& fragment)
{
	this->states_.insert(fragment.states_.begin(), fragment.states_.end());
	this->transitions_.insert(fragment.transitions_.begin(), fragment.transitions_.end());

	this->transitions_[MoveFunctionArgument(this->outcomingHalfTrans_.fromStateName_, this->outcomingHalfTrans_.inputSymbol_)]
		.insert(fragment.incomingHalfTrans_.toStateName_);

	this->outcomingHalfTrans_ = fragment.outcomingHalfTrans_;

	return *this;
}

NFAFragment& NFAFragment::makeStar()
{
	//NFAFragment epsFrag = NFAFragment::createNFAForEpsilon();

	State newState(nfaStateNamesGen_.getNextAvailableName());

	this->states_.insert(newState);
	
	this->transitions_[MoveFunctionArgument(newState.name_, '#')]
		.insert(this->incomingHalfTrans_.toStateName_);
	this->transitions_[MoveFunctionArgument(this->outcomingHalfTrans_.fromStateName_, this->outcomingHalfTrans_.inputSymbol_)]
		.insert(newState.name_);

	this->initStateName_ = newState.name_;
	
	this->incomingHalfTrans_.toStateName_ = newState.name_;

	this->outcomingHalfTrans_.fromStateName_ = newState.name_;
	this->outcomingHalfTrans_.inputSymbol_ = '#';

	return *this;
}

NFAFragment& NFAFragment::makePlus()
{
	State leftNewState = State(nfaStateNamesGen_.getNextAvailableName());
	State rightNewState = State(nfaStateNamesGen_.getNextAvailableName());

	this->states_.insert(leftNewState);
	this->states_.insert(rightNewState);

	this->transitions_[MoveFunctionArgument(leftNewState.name_, '#')]
		.insert(this->incomingHalfTrans_.toStateName_);
	this->transitions_[MoveFunctionArgument(this->outcomingHalfTrans_.fromStateName_, this->outcomingHalfTrans_.inputSymbol_)]
		.insert(rightNewState.name_);
	this->transitions_[MoveFunctionArgument(rightNewState.name_, '#')]
		.insert(leftNewState.name_);

	this->initStateName_ = leftNewState.name_;

	this->incomingHalfTrans_.toStateName_ = leftNewState.name_;

	this->outcomingHalfTrans_.fromStateName_ = rightNewState.name_;
	this->outcomingHalfTrans_.inputSymbol_ = '#';

	return *this;
}

NFAFragment& NFAFragment::makeQuestionMark()
{
	this->transitions_[MoveFunctionArgument(this->incomingHalfTrans_.toStateName_, '#')]
		.insert(this->outcomingHalfTrans_.fromStateName_);

	return *this;
}

NFAFragment& NFAFragment::addAcceptingState(const std::string& stateName)
{
	State acceptingState(stateName, true);

	this->states_.insert(acceptingState);
	this->transitions_[MoveFunctionArgument(this->outcomingHalfTrans_.fromStateName_, this->outcomingHalfTrans_.inputSymbol_)]
		.insert(acceptingState.name_);

	this->outcomingHalfTrans_.fromStateName_ = acceptingState.name_;
	this->outcomingHalfTrans_.inputSymbol_ = '#'; // ??!!;

	return *this;
}

NFAFragment& NFAFragment::operator = (NFAFragment&& frag)
{
	if (this != &frag)
	{
		this->FiniteAutomaton::operator=(frag);
		this->incomingHalfTrans_ = move(frag.incomingHalfTrans_);
		this->outcomingHalfTrans_ = move(frag.outcomingHalfTrans_);
		this->transitions_ = move(frag.transitions_);

		//cout << "NFAFragment::move-assign\n";
	}

	return *this;
}



// reading / writing;
ostream& operator<<(ostream& os, const NFAFragment& nfaFrag)
{
	os << *(FiniteAutomaton*)(&nfaFrag);
	
	os << "   Incoming half-transition:\n"
		<< "To-state - " << nfaFrag.incomingHalfTrans_.toStateName_ << '\n';

	os << "   Outcoming half-transition:\n"
		<< "From-state - " << nfaFrag.outcomingHalfTrans_.fromStateName_ << '\n'
		<< "Input symbol - " << nfaFrag.outcomingHalfTrans_.inputSymbol_ << '\n';

	os << "   Transitions: \n";
	for each (auto elem in nfaFrag.transitions_)
	{
		os << elem.first.fromStateName_ << ' ' << elem.first.inputSymbol_ << "  ->  ";
		copy(elem.second.begin(), elem.second.end(), ostream_iterator<string>(os, " "));
		os << '\n';
	}

	return os;
}

istream& operator>>(istream& is, NFAFragment& nfaFrag)
{
	is >> *(FiniteAutomaton*)(&nfaFrag);

	string line = "";
	string titleString = "";
	string stateName = "";

	// reading incoming half-transition;
	is >> titleString;
	is >> stateName;
	nfaFrag.incomingHalfTrans_ = IncomingHalfTransition(stateName);

	// reading outcoming half-transition;
	is >> titleString;
	is >> stateName;
	char inputSymbol;
	is >> inputSymbol;
	nfaFrag.outcomingHalfTrans_ = OutcomingHalfTransition(stateName, inputSymbol);


	// reading transitions;
	map <MoveFunctionArgument, set<string> > transitions;

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

		set<string> toStateNames;
		while (myStream >> stateName)
		{
			toStateNames.insert(stateName);
		}

		transitions[MoveFunctionArgument(fromStateName, inputSymbol)] = toStateNames;
	}

	nfaFrag.transitions_ = transitions;

	return is;
}