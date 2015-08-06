#pragma once
#include <iostream>
#include <map>
#include "Visitor.h"
using namespace std;

class TreeSerializer : public Visitor
{
	int level;
	multimap <string, Instructions::Expressions::Storage*>* storages;
	ostream* os;
public:
	TreeSerializer(multimap <string, Instructions::Expressions::Storage*>* _storages, ostream& _os);

	virtual void visit(Instruction* root) ;
	virtual void visit(Instructions::Block* root);
	virtual void visit(Instructions::Definition* root);
	virtual void visit(Instructions::Expression* root);
	virtual void visit(Instructions::Expressions::Operator* root);
	virtual void visit(Instructions::Expressions::Operators::Binary::Assignment* root);
	virtual void visit(Instructions::Expressions::Storage* root);
	virtual void visit(Instructions::Expressions::Storages::Literal* root);
	virtual void visit(Instructions::Expressions::Storages::Variable* root);
	virtual void visit(Instructions::Expressions::Storages::In* root);
	virtual void visit(Instructions::Expressions::Storages::Out* root);
	virtual void visit(Instructions::Expressions::Operators::BinaryOperator* root);
	virtual void visit(Instructions::Expressions::Operators::Binary::Plus* root);
	virtual void visit(Instructions::Expressions::Operators::Binary::Minus* root);
	virtual void visit(Instructions::Expressions::Operators::Binary::Multiply* root);
	virtual void visit(Instructions::Expressions::Operators::Binary::Divide* root);

	void tabulate();
};