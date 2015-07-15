#pragma once
#include "FiniteAutomaton.h"
#include "DFA.h"

#include <vector>

// utility functions;
bool isCharDigit(const char ch);
bool isStringInteger(const std::string& s);

// for generating unique state names (significant for NFA's combinators and generators);
class StateNamesGenerator
{
	// statics;
private:
	static const std::string digitToChar(int digit);
	static const std::string intToString(int i);

private:
	int counter_;

public:
	StateNamesGenerator();

	const std::string getNextAvailableName();
};



struct MarkableStateName
{
	std::string stateName_;
	bool isMarked_;

	MarkableStateName(const std::string& stateName = "", const bool isMarked = false);

	bool operator<(const MarkableStateName& msn) const;
};





// NFA Fragment for combining to NFA;
class NFAFragment
	:public FiniteAutomaton
{
	// static members;
private:
	static StateNamesGenerator nfaStateNamesGen_;
	static StateNamesGenerator dfaStateNamesGen_;

public:
	static NFAFragment createNFAForEpsilon();
	static NFAFragment createNFAForSingleSymbol(const char symbol);
	static NFAFragment createNFAForString(const std::string& str);
	static NFAFragment createNFAForRange(const char first, const char last); // [first, last];
	static NFAFragment createNFAForRange(const std::vector<char>& symbols);

	static NFAFragment makeParallel(std::vector<NFAFragment> frags);


private:
	IncomingHalfTransition incomingHalfTrans_;
	OutcomingHalfTransition outcomingHalfTrans_;
	std::map<MoveFunctionArgument, std::set<std::string> > transitions_;

private:
	typedef std::map<std::string, std::string> NfaDfaStatesMap;

	const std::string getEpsilonClosure(const std::string& stateName/*, DfaNfaStatesMap& dfaNfaStatesMap*/) const;
	const std::string getReachableThroughS(const std::string& stateName, const char S/*, DfaNfaStatesMap& dfaNfaStatesMap*/) const;
	const std::set<char> getExternalAlphabet() const;

public:

	NFAFragment();
	NFAFragment(const NFAFragment& frag);
	NFAFragment(NFAFragment&& frag);

	const DFA convertToDFA() const;

	// compositional modifying methods;
	NFAFragment& makeConcatenation(const NFAFragment& fragment); // +
	// makeParallel - static;
	NFAFragment& makeStar(); // +
	NFAFragment& makePlus(); // +
	NFAFragment& makeQuestionMark();
	NFAFragment& addAcceptingState(const Token::Type acceptingTokenType); // +

	NFAFragment& operator=(NFAFragment&& frag);

	friend std::ostream& operator<<(std::ostream& os, const NFAFragment& nfaFrag);
	friend std::istream& operator>>(std::istream& is, NFAFragment& nfaFrag);
};