#include <string>
#include <vector>
#include <set>
#include <stack>
#include "ParserClasses.h"
#include "Token.h"
#include "Parser.h"
using namespace std;

string operatorTypes[] =
{
	"!", "&&", "||", "<", ">", "==", "!=", "<=", ">=", "+", "-", "*", "/", "^"
};
string operatorTypes2[] =
{
	"not", "and", "or"
};
int operatorTypes2size = 3;
string variableTypes[] =
{
	"bool", "int", "uint", "float", "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4", "mat2", "mat3", "mat4", "quat", "1D", "2D", "3D", "Cube"
};


Block* Parser::parse()
{
	tokens.insert(tokens.begin(), Token::createToken(Token::Type::Operator, "{"));
	tokens.push_back(Token::createToken(Token::Type::Operator, "}"));
	return isBlock(tokens.begin());
}

bool Parser::isOperator(TokenIter& iter)
{
	return (*iter)->getTokenType() == Token::Type::Operator;
}
bool Parser::isOperator(TokenIter& iter, string op)
{
	if (isOperator(iter) &&
		((OperatorToken*)(*iter))->getValue() == op)
	{
		iter++;
		return true;
	}
	else return false;
}


bool Parser::isKeyword(TokenIter& iter)
{
	return (*iter)->getTokenType() == Token::Type::Keyword;
}
bool Parser::isKeyword(TokenIter& iter, string kw)
{
	if (isKeyword(iter) &&
		((OperatorToken*)(*iter))->getValue() == kw)
	{
		iter++;
		return true;
	}
	else return false;
}

bool Parser::isIdentifier(TokenIter& iter)
{
	return ((*iter)->getTokenType() == Token::Type::Identifier);
}

string Parser::getIdentifier(TokenIter& iter)
{
	string name = ((IdentifierToken*)(*iter))->getValue();
	iter++;
	return name;
}

Block* Parser::isBlock(TokenIter& iter)
{
	if (!isOperator(iter, "{"))
		return nullptr;

	Block* block = new Block();
	Instruction* instruction;
	while (instruction = isInstruction(iter))
		block->instructions.push_back(instruction);

	if (!isOperator(iter, "}"))
		throw ParserError(*iter, "Expecting '}'");

	return block;
}

Instruction* Parser::isInstruction(TokenIter& iter)
{
	Instruction* instruction;
	if ((instruction = isDeclaration(iter)) ||
		(instruction = isAssignement(iter)) ||
		(instruction = isIfStatement(iter)))
		return instruction;
	if (isEmptyInstruction(iter))
		return new Instruction();
	return nullptr;
}

bool Parser::isEmptyInstruction(TokenIter& iter)
{
	return isOperator(iter, ";");
}

IntegerLiteral* Parser::isIntegerLiteral(TokenIter& iter)
{
	if (!((*iter)->getTokenType() == Token::Type::IntegerLiteral))
		return nullptr;
	IntegerLiteral* integer = new IntegerLiteral();
	integer->value = ((IntegerLiteralToken*)(*iter))->getValue();
	iter++;
	return integer;
}
RealLiteral* Parser::isRealLiteral(TokenIter& iter)
{
	if (!((*iter)->getTokenType() == Token::Type::RealLiteral))
		return nullptr;
	RealLiteral* real = new RealLiteral();
	real->value = ((RealLiteralToken*)(*iter))->getValue();
	iter++;
	return real;
}
BoolLiteral* Parser::isBoolLiteral(TokenIter& iter)
{
	if (isKeyword(iter, "true"))
	{
		BoolLiteral* bl = new BoolLiteral();
		bl->value = true;
		return bl;
	}
	else if (isKeyword(iter, "false"))
	{
		BoolLiteral* bl = new BoolLiteral();
		bl->value = false;
		return bl;
	}
	else return nullptr;
}
Literal* Parser::isLiteral(TokenIter& iter)
{
	Literal* lit;
	if ((lit = isIntegerLiteral(iter)) ||
		(lit = isRealLiteral(iter)) ||
		(lit = isBoolLiteral(iter)))
		return lit;
	else return nullptr;
}

InOutQualifier* Parser::isInOutQualifier(TokenIter& iter)
{
	auto qualifier = new InOutQualifier();
	if (isKeyword(iter, "in"))
		qualifier->type = InOutType::_in;
	else if (isKeyword(iter, "out"))
		qualifier->type = InOutType::_out;
	else return nullptr;
	if (isOperator(iter, "["))
	{
		if (auto index = isIntegerLiteral(iter))
			qualifier->index = index;
		else throw ParserError(*iter, "IN/OUT index can be only integer literal");

		if (!isOperator(iter, "]"))
			throw ParserError(*iter, "Expecting ']'");
	}
	return qualifier;
}
Qualifier* Parser::isQualifier(TokenIter& iter)
{
	if (isKeyword(iter, "extern"))
		return new ExternQualifier();
	if (auto InOut = isInOutQualifier(iter))
		return InOut;
	return nullptr;
}
VariableType* Parser::isTypeName(TokenIter& iter)
{
	for (int i = 0; i < (int)VariableType::__size__; ++i)
		if (isKeyword(iter, variableTypes[i]))
			return new VariableType((VariableType)i);
	return nullptr;
}
Declaration* Parser::isDeclaration(TokenIter& iter)
{
	Declaration* decl = new Declaration();
	decl->qualifier = nullptr;
	if (auto qualif = isQualifier(iter))
		decl->qualifier = qualif;
	if (auto type = isTypeName(iter))
	{
		decl->type = *type;
		delete type;
	}
	else if (decl->qualifier)
		throw ParserError(*iter, "Expecting identifier type");
	else return nullptr;

	if (isIdentifier(iter))
		decl->name = getIdentifier(iter);
	else throw ParserError(*iter, "Expecting identifier name");

	if (isOperator(iter, "="))
		if (auto init = isExpression(iter))
		{
			Definition* def = new Definition();
			def->name = decl->name;
			def->type = def->type;
			def->qualifier = decl->qualifier;
			def->initialization = init;
			delete decl;
			decl = def;
		}
		else throw ParserError(*iter, "Expecting initialization");

	if (!isOperator(iter, ";"))
		throw ParserError(*iter, "Expecting ';'");
	return decl;
}
string Properties[] = {
	"red", "green", "blue", "alpha"
};
Variable* Parser::isVariable(TokenIter& iter)
{
	string name;
	IntegerLiteral *x = nullptr, *y = nullptr;
	if (isIdentifier(iter))
		name = getIdentifier(iter);
	else return nullptr;
	if (isOperator(iter, "["))
	{
		if (!(x = isIntegerLiteral(iter)))
			throw ParserError(*iter, "Expecting integer literal");
		if (!isOperator(iter, "]"))
			throw ParserError(*iter, "Expecting ']'");
		if (isOperator(iter, "["))
		{
			if (!(y = isIntegerLiteral(iter)))
				throw ParserError(*iter, "Expecting integer literal");
			if (!isOperator(iter, "]"))
				throw ParserError(*iter, "Expecting ']'");
			MatVariable* var = new MatVariable();
			var->name = name;
			var->xIndex = x;
			var->yIndex = y;
			return var;
		} 
		else if (isOperator(iter, "."))
		{
			for (int i = 0; i < 4; ++i)
				if (isKeyword(iter, Properties[i]))
				{
					y = new IntegerLiteral();
					y->value = i;
				}
			if (y == nullptr)
				throw ParserError(*iter, "Expecting red, gree, blue or alpha");
			MatVariable* var = new MatVariable();
			var->name = name;
			var->xIndex = x;
			var->yIndex = y;
			return var;
		}
		else
		{
			VecVariable* var = new VecVariable();
			var->name = name;
			var->index = x;
			return var;
		}
	}
	else if (isOperator(iter, "."))
	{
		for (int i = 0; i < 4; ++i)
		if (isKeyword(iter, Properties[i]))
		{
			x = new IntegerLiteral();
			x->value = i;
		}
		if (x == nullptr)
			throw ParserError(*iter, "Expecting red, gree, blue or alpha");
		if (isOperator(iter, "["))
		{
			if (!(y = isIntegerLiteral(iter)))
				throw ParserError(*iter, "Expecting integer literal");
			if (!isOperator(iter, "]"))
				throw ParserError(*iter, "Expecting ']'");
			MatVariable* var = new MatVariable();
			var->name = name;
			var->xIndex = x;
			var->yIndex = y;
			return var;
		}
		else if (isOperator(iter, "."))
		{
			for (int i = 0; i < 4; ++i)
			if (isKeyword(iter, Properties[i]))
			{
				y = new IntegerLiteral();
				y->value = i;
			}
			if (y == nullptr)
				throw ParserError(*iter, "Expecting red, gree, blue or alpha");
			MatVariable* var = new MatVariable();
			var->name = name;
			var->xIndex = x;
			var->yIndex = y;
			return var;
		}
		else
		{
			VecVariable* var = new VecVariable();
			var->name = name;
			var->index = x;
			return var;
		}
	}
	else if (isOperator(iter, "{"))
	{
		Expression *xe, *ye, *ze;
		if (!(xe = isExpression(iter)))
			throw ParserError(*iter, "Expecting expression");
		if (isOperator(iter, "}"))
		{
			TexVariable* var = new TexVariable();
			var->name = name;
			var->xIndex = xe;
			return var;
		}
		else if (isOperator(iter, ","))
		{
			if (!(ye = isExpression(iter)))
				throw ParserError(*iter, "Expecting expression");
			if (isOperator(iter, "}"))
			{
				Tex2DVariable* var = new Tex2DVariable();
				var->name = name;
				var->xIndex = xe;
				var->yIndex = ye;
				return var;
			}
			else if (isOperator(iter, ","))
			{
				if (!(ze = isExpression(iter)))
					throw ParserError(*iter, "Expecting expression");
				if (isOperator(iter, "}"))
				{
					Tex3DVariable* var = new Tex3DVariable();
					var->name = name;
					var->xIndex = xe;
					var->yIndex = ye;
					var->zIndex = ze;
					return var;
				}
				else throw ParserError(*iter, "Expecting '}'");
			}
			else throw ParserError(*iter, "Expecting '}' or ','");
		}
		else throw ParserError(*iter, "Expecting '}' or ','");
	}
	else
	{
		Variable* var = new Variable();
		var->name = name;
		return var;
	}
}
Assignement* Parser::isAssignement(TokenIter& iter)
{
	Assignement* assig = new Assignement();
	if (isIdentifier(iter))
	{
		assig->to = new Variable();
		assig->to->name = getIdentifier(iter);
	}
	else return nullptr;

	if (!isOperator(iter, "="))
	{
		iter--;
		delete assig->to;
		delete assig;
		return nullptr;
	}
	if (auto expr = isExpression(iter))
		assig->from = expr;
	else throw ParserError(*iter, "Expecting expression");
	
	if (!isOperator(iter, ";"))
		throw ParserError(*iter, "Expecting ';'");

	return assig;
}
IfStatement* Parser::isIfStatement(TokenIter& iter)
{
	if (!isKeyword(iter, "if"))
		return nullptr;
	if (!isOperator(iter, "("))
		throw ParserError(*iter, "Expecting '('");

	IfStatement* ifs = new IfStatement();
	if (auto cond = isExpression(iter))
		ifs->condition = cond;
	else throw ParserError(*iter, "Expecting '('");

	if (!isOperator(iter, ")"))
		throw ParserError(*iter, "Expecting ')'");

	if (auto instr = isInstruction(iter))
		ifs->ifTrue = instr;
	else if (auto block = isBlock(iter))
		ifs->ifTrue = block;
	else throw ParserError(*iter, "Expecting instruction or block");

	if (isKeyword(iter, "else"))
	{
		IfElseStatement* ifes = new IfElseStatement();
		ifes->condition = ifs->condition;
		ifes->ifTrue = ifs->ifTrue;
		delete ifs;
		if (auto instr = isInstruction(iter))
			ifes->ifFalse = instr;
		else if (auto block = isBlock(iter))
			ifes->ifFalse = block;
		else throw ParserError(*iter, "Expecting instruction or block");
		ifs = ifes;
	}

	return ifs;
}
Function* Parser::isFunctionToken(TokenIter& iter)
{
	TokenIter next = iter + 1;
	if (isIdentifier(iter) && isOperator(next, "("))
	{
		Function* func = new Function();
		func->name = getIdentifier(iter);
		return func;
	}
	else return nullptr;
}
Operator* Parser::isOperatorToken(TokenIter& iter)
{
	for (int i = 0; i < (int)OperatorType::__size__; ++i)
		if (isOperator(iter, operatorTypes[i]) || (i<operatorTypes2size && isOperator(iter, operatorTypes[i])))
		{
			Operator* oper = new Operator();
			oper->type = (OperatorType)i;
			return oper;
		}
		return nullptr;
}
struct OpStackElement
{
	enum Type {
		_Operator, _Function, _Parenthesis
	} type;
	union Value
	{
		Operator* _operator;
		Function* _function;
	} value;
};
Expression* Parser::isExpression(TokenIter& iter)
{
	if (auto asig = isAssignement(iter))
		return asig;
	stack<OpStackElement> opStack;
	stack<Expression*> out;
	TokenIter tmp = iter;
	int parenthesisCount = 0;
	while (iter != tokens.end())
	{
		if (auto lit = isLiteral(iter))
			out.push(lit);
		else if (auto var = isVariable(iter))
			out.push(var);
		else if (auto func = isFunctionToken(iter))
		{
			func->arguments.push_back(nullptr); // argument reservation
			OpStackElement elem;
			elem.type = OpStackElement::Type::_Function;
			elem.value._function = func;
			opStack.push(elem);
		}
		else if (isOperator(iter, ","))
		{
			while (!opStack.empty() && opStack.top().type != OpStackElement::Type::_Parenthesis)
			{
				if (opStack.top().type != OpStackElement::Type::_Operator)
					throw ParserError(Token::createToken(Token::Type::Identifier, opStack.top().value._function->name),
					"Expecting operator");
				Operator* curr = opStack.top().value._operator; opStack.pop();
				curr->operand2 = out.top(); out.pop();
				curr->operand1 = out.top(); out.pop();
				out.push(curr);
			}
			if (opStack.empty()) // may be comma expression
			{
				iter--;
				break;
			}
			OpStackElement par = opStack.top();
			opStack.pop();
			if (opStack.top().type != OpStackElement::Type::_Function)
				throw ParserError(*iter, "Separator can be used only in function calls");
			opStack.top().value._function->arguments.push_back(nullptr); // argument reservation
			opStack.push(par);
		}
		else if (auto op = isOperatorToken(iter))
		{
			while (!opStack.empty() && opStack.top().type == OpStackElement::Type::_Operator &&
				op->type <= opStack.top().value._operator->type)
			{
				Operator* curr = opStack.top().value._operator; opStack.pop();
				curr->operand2 = out.top(); out.pop();
				curr->operand1 = out.top(); out.pop();
				out.push(curr);
			}
			OpStackElement elem;
			elem.type = OpStackElement::Type::_Operator;
			elem.value._operator = op;
			opStack.push(elem);
		}
		else if (isOperator(iter, "("))
		{
			OpStackElement elem;
			elem.type = OpStackElement::Type::_Parenthesis;
			opStack.push(elem);
			parenthesisCount++;
		}
		else if (isOperator(iter, ")"))
		{
			if (parenthesisCount == 0)
			{
				iter--;
				break;
			}

			parenthesisCount--;
			while (!opStack.empty() && opStack.top().type != OpStackElement::Type::_Parenthesis)
			{
				if (opStack.top().type != OpStackElement::Type::_Operator)
					throw ParserError(Token::createToken(Token::Type::Identifier, opStack.top().value._function->name), 
						"Expecting operator");
				Operator* curr = opStack.top().value._operator; opStack.pop();
				curr->operand2 = out.top(); out.pop();
				curr->operand1 = out.top(); out.pop();
				out.push(curr);
			}
			if (!opStack.empty())
				opStack.pop();
			else throw ParserError(*iter, "Parenthesis mismatch");
			if (opStack.top().type == OpStackElement::Type::_Function)
			{
				Function* func = opStack.top().value._function;
				for (int i = func->arguments.size() - 1; i >= 0; --i)
				{
					func->arguments[i] = out.top();
					out.pop();
				}
				out.push(func);
			}
		}
		else break;
	}
	while (!opStack.empty() && opStack.top().type == OpStackElement::Type::_Operator)
	{
		Operator* curr = opStack.top().value._operator; opStack.pop();
		curr->operand2 = out.top(); out.pop();
		curr->operand1 = out.top(); out.pop();
		out.push(curr);
	}
	if (!opStack.empty())
		throw ParserError(Token::createToken(Token::Type::Operator, "("),
			"Parenthesis mismatch");
	if (out.empty())
	{
		iter = tmp;
		return nullptr;
	}
	if (out.size() != 1)
		throw ParserError(Token::createToken(Token::Type::Operator, "?"),
			"Error in expression");
	return out.top();
}