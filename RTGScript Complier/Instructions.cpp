#include "Instructions.h"


#include "Translator.h"
#include "Visitor.h"

void					Instruction::Accept(Translator* translator)
{
	//translator->translate(this);
}	
void					Instructions::Block::Accept(Translator* translator)
{	
	//translator->translate(this);
}	
void					Instructions::Definition::Accept(Translator* translator)
{	
	//translator->translate(this);
}
void					Instructions::Expression::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Operator::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Operators::Binary::Assignment::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Storage::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Storages::Literal::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Storages::Variable::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Storages::In::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Storages::Out::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Operators::BinaryOperator::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Operators::Binary::Plus::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Operators::Binary::Minus::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Operators::Binary::Multiply::Accept(Translator* translator)
{
	//translator->translate(this);
}
void					Instructions::Expressions::Operators::Binary::Divide::Accept(Translator* translator)
{
	//translator->translate(this);
}

//------------------------- Visitor

void					Instruction::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Block::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Definition::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expression::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Operator::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Operators::Binary::Assignment::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Storage::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Storages::Literal::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Storages::Variable::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Storages::In::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Storages::Out::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Operators::BinaryOperator::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Operators::Binary::Plus::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Operators::Binary::Minus::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Operators::Binary::Multiply::Accept(Visitor* visitor)
{
	visitor->visit(this);
}
void					Instructions::Expressions::Operators::Binary::Divide::Accept(Visitor* visitor)
{
	visitor->visit(this);
}

std::string Expressions::Storage::TypeNames[(unsigned int)Expressions::Storage::Type::__size__] = {
	"bool", "int", "float",
	"vec2", "ivec2",
	"vec3", "ivec3",
	"vec4", "ivec4",
	"mat2", "mat3", "mat4",
	"1D", "2D", "3D", "Cube"
};

Block::~Block()
{
	for (auto i : content)
		delete i;
}
Definition::~Definition()
{
	//delete storage;
	if (initializer)
		delete initializer;
}
Expressions::Storages::Literal::~Literal()
{
	delete value;
}
Expressions::Operators::BinaryOperator::~BinaryOperator()
{
	delete left;
	delete right;
}
Expressions::Operators::Binary::Assignment::~Assignment()
{
	//delete left;
	delete right;
}
