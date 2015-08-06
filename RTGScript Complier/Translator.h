#pragma once
#include <iostream>
#include <vector>
#include <string>


#include "Parser.h"


struct Translator
{
protected:

	enum class Operation
	{
		None,
		Receiving,
		Assignation
	};

	Instructions::Expressions::Storage::Type operationType;
	Operation								operation = Operation::None;
	std::vector<uint8>*						code;

public:
	void translate(Instruction* root);
	void translate(Instructions::Block* root);
	void translate(Instructions::Definition* root);
	void translate(Instructions::Expression* root);
	void translate(Instructions::Expressions::Operator* root);
	void translate(Instructions::Expressions::Operators::Binary::Assignment* root);
	void translate(Instructions::Expressions::Storage* root);
	void translate(Instructions::Expressions::Storages::Literal* root);
	void translate(Instructions::Expressions::Storages::Variable* root);
	void translate(Instructions::Expressions::Storages::In* root);
	void translate(Instructions::Expressions::Storages::Out* root);

	std::vector<uint8> TranslateToASM86(Instruction* root);
};

