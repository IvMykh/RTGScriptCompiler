#pragma once
#include <string>
#include <vector>
using namespace std;

struct Instruction
{
	virtual void serialize(ostream& os, int level) {};
};

struct Block : public Instruction
{
	vector<Instruction*> instructions;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Block";
		for (auto i : instructions)
			i->serialize(os, level + 1);
	}
};

struct Expression : public Instruction
{
};

struct Literal : public Expression
{
};
struct IntegerLiteral : public Literal
{
	int value;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "IntegerLiteral " << value;
	}
};
struct RealLiteral : public Literal
{
	float value;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "RealLiteral " << value;
	}
};
struct BoolLiteral : public Literal
{
	bool value;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "BoolLiteral " << boolalpha << value;
	}
};

struct Qualifier
{
	virtual void serialize(ostream& os, int level) {};
};
struct ExternQualifier : public Qualifier
{
};
enum class InOutType
{
	_in, _out
};
struct InOutQualifier : public Qualifier
{
	InOutType type;
	IntegerLiteral* index;
	virtual void serialize(ostream& os, int level) override{
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os <<( (type == InOutType::_in) ? "IN" : "OUT" ) << " Qualifier";
		index->serialize(os, level + 1);
	}
};

struct Variable : public Expression
{
	string name;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Variable '" << name << "'";
	}
};
struct VecVariable : public Variable
{
	IntegerLiteral* index;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "VecVariable '" << name << "'";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  index:";
		index->serialize(os, level + 1);
	}
};
struct MatVariable : public Variable
{
	IntegerLiteral* xIndex;
	IntegerLiteral* yIndex;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "MatVariable '" << name << "'";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  xIndex:";
		xIndex->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  yIndex:";
		yIndex->serialize(os, level + 1);
	}
};
struct TexVariable : public Variable
{
	Expression* xIndex;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "TexVariable '" << name << "'";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  xIndex:";
		xIndex->serialize(os, level + 1);
	}
};
struct Tex2DVariable : public TexVariable
{
	Expression* yIndex;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Tex2DVariable '" << name << "'";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  xIndex:";
		xIndex->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  yIndex:";
		yIndex->serialize(os, level + 1);
	}
};
struct Tex3DVariable : public Tex2DVariable
{
	Expression* zIndex;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "VecVariable '" << name << "'";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  xIndex:";
		xIndex->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  yIndex:";
		yIndex->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  zIndex:";
		zIndex->serialize(os, level + 1);
	}
};
enum class CubeSide
{
	_left, _right, _top, _bottom, _front, _back
};
struct TexCubeVariable : public Tex2DVariable
{
	CubeSide side;
};

enum class OperatorType
{
	_not, _and, _or, _less, _greater, _equal, _notEqual, _lessOrEqual, _greaterOrEqual, _plus, _minus, _multiply, _divide, _power, __size__
};
/*
string operatorTypes[] =
{
	"!", "&&", "||", "<", ">", "==", "!=", "<=", ">=", "+", "-", "*", "/", "^"
};
string operatorTypes2[] =
{
	"not", "and", "or"
};
int operatorTypes2size = 3;
*/
struct Operator : public Expression
{
	OperatorType type;
	Expression* operand1;
	Expression* operand2;
	virtual void serialize(ostream& os, int level) override {
		string operatorTypes[] =
		{
			"!", "&&", "||", "<", ">", "==", "!=", "<=", ">=", "+", "-", "*", "/", "^"
		};
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Operator '" << operatorTypes[(int)type] << "'";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  operand1:";
		operand1->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  operand2:";
		operand2->serialize(os, level + 1);
	}
};
struct Function : public Expression
{
	string name;
	vector<Expression*> arguments;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Function '" << name << "' + arguments";
		for (auto i : arguments)
			i->serialize(os, level + 1);
	}
};
enum class VariableType
{
	_bool, _int, _uint, _float, _vec2, _vec3, _vec4, _ivec2, _ivec3, _ivec4, _mat2, _mat3, _mat4, _quat, _1D, _2D, _3D, _Cube, __size__
};
/*
string variableTypes[] =
{
	"bool", "int", "uint", "float", "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4", "mat2", "mat3", "mat4", "quat", "1D", "2D", "3D", "Cube"
};
*/
struct Declaration : public Instruction
{
	Qualifier* qualifier;
	VariableType type;
	string name;
	virtual void serialize(ostream& os, int level) override {
		string variableTypes[] =
		{
			"bool", "int", "uint", "float", "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4", "mat2", "mat3", "mat4", "quat", "1D", "2D", "3D", "Cube"
		};
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Declaration '" << name << "' of type " << variableTypes[(int)type];
		if (qualifier)
		{
			os << endl; for (int i = 0; i < level; ++i) os << "    ";
			os << "  qualifier:";
			qualifier->serialize(os, level + 1);
		}
	}
};
struct Definition : public Declaration
{
	Expression* initialization;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Definition '" << name << "' of type " << (int)type;
		if (qualifier)
		{
			os << endl; for (int i = 0; i < level; ++i) os << "    ";
			os << "  qualifier:";
			qualifier->serialize(os, level + 1);
		}
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  initialization:";
		initialization->serialize(os, level + 1);
	}
};

struct Assignement : public Expression
{
	Variable* to;
	Expression* from;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "Assignement '";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  to:";
		to->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  from:";
		from->serialize(os, level + 1);
	}
};

struct IfStatement : public Instruction
{
	Expression* condition;
	Instruction* ifTrue;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "IF statement '";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  condition:";
		condition->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  on TRUE:";
		ifTrue->serialize(os, level + 1);
	}
};
struct IfElseStatement : public IfStatement
{
	Instruction* ifFalse;
	virtual void serialize(ostream& os, int level) override {
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "IF+ELSE statement '";
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  condition:";
		condition->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  on TRUE:";
		ifTrue->serialize(os, level + 1);
		os << endl; for (int i = 0; i < level; ++i) os << "    ";
		os << "  on FALSE:";
		ifFalse->serialize(os, level + 1);
	}
};
