#pragma once
#include "Instructions.h"

class Visitor
{
public:
	virtual void visit(Instruction* root) = 0;
	virtual void visit(Instructions::Block* root) = 0;
	virtual void visit(Instructions::Definition* root) = 0;
	virtual void visit(Instructions::Expression* root) = 0;
	virtual void visit(Instructions::Expressions::Operator* root) = 0;
	virtual void visit(Instructions::Expressions::Operators::Binary::Assignment* root) = 0;
	virtual void visit(Instructions::Expressions::Storage* root) = 0;
	virtual void visit(Instructions::Expressions::Storages::Literal* root) = 0;
	virtual void visit(Instructions::Expressions::Storages::Variable* root) = 0;
	virtual void visit(Instructions::Expressions::Storages::In* root) = 0;
	virtual void visit(Instructions::Expressions::Storages::Out* root) = 0;
	virtual void visit(Instructions::Expressions::Operators::BinaryOperator* root) = 0;
	virtual void visit(Instructions::Expressions::Operators::Binary::Plus* root) = 0;
	virtual void visit(Instructions::Expressions::Operators::Binary::Minus* root) = 0;
	virtual void visit(Instructions::Expressions::Operators::Binary::Multiply* root) = 0;
	virtual void visit(Instructions::Expressions::Operators::Binary::Divide* root) = 0;
};