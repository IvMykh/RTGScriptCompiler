#include "TreeSerializer.h"
#include <algorithm>

TreeSerializer::TreeSerializer(multimap <string, Instructions::Expressions::Storage*>* _storages, ostream& _os) :
	storages(_storages),
	os(&_os),
	level(0)
{
}

void TreeSerializer::tabulate()
{
	for (int i = 0; i < level; ++i)
		(*os) << "    ";
}

void TreeSerializer::visit(Instruction* root)
{
	tabulate();
	(*os) << "Instruction" << endl;
}
void TreeSerializer::visit(Instructions::Block* root)
{
	tabulate();
	(*os) << "Block" << endl;
	level++;
	for (auto instr : root->content)
		instr->Accept(this);
	level--;
}
void TreeSerializer::visit(Instructions::Definition* root)
{
	tabulate();
	(*os) << "Definition" << endl;
	level++;
	root->storage->Accept(this);
	if (root->initializer)
	{
		tabulate();
		(*os) << " (initializer)" << endl;
		root->initializer->Accept(this);
	}
	level--;
}
void TreeSerializer::visit(Instructions::Expression* root)
{
	tabulate();
	(*os) << "Expression" << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Operator* root)
{
	tabulate();
	(*os) << "Operator" << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Operators::Binary::Assignment* root)
{
	tabulate();
	(*os) << "Assignment" << endl;
	level++;
	tabulate();
	(*os) << " (left)" << endl;
	root->left->Accept(this);
	tabulate();
	(*os) << " (right)" << endl;
	root->right->Accept(this);
	level--;
}
void TreeSerializer::visit(Instructions::Expressions::Storage* root)
{
	tabulate();
	(*os) << "Storage" << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Storages::Literal* root)
{
	tabulate();
	(*os) << "Literal ";
	switch (root->GetStorageType())
	{
	case Instructions::Expressions::Storage::Type::Bool:
		(*os) << boolalpha << *(bool*)root->value;
		break;
	case Instructions::Expressions::Storage::Type::Float:
		(*os) << *(float*)root->value;
		break;
	case Instructions::Expressions::Storage::Type::Int:
		(*os) << *(int*)root->value;
		break;
	case Instructions::Expressions::Storage::Type::IVec2:
		(*os) << "ivec2(" << *(ivec2*)root->value << ")";
		break;
	case Instructions::Expressions::Storage::Type::IVec3:
		(*os) << "ivec3(" << *(ivec3*)root->value << ")";
		break;
	case Instructions::Expressions::Storage::Type::IVec4:
		(*os) << "ivec4(" << *(ivec4*)root->value << ")";
		break;
	case Instructions::Expressions::Storage::Type::Mat2:
		(*os) << "mat2(...)";
		break;
	case Instructions::Expressions::Storage::Type::Mat3:
		(*os) << "mat3(...)";
		break;
	case Instructions::Expressions::Storage::Type::Mat4:
		(*os) << "mat4(...)";
		break;
	case Instructions::Expressions::Storage::Type::Vec2:
		(*os) << "vec2(" << *(vec2*)root->value << ")";
		break;
	case Instructions::Expressions::Storage::Type::Vec3:
		(*os) << "vec3(" << *(vec3*)root->value << ")";
		break;
	case Instructions::Expressions::Storage::Type::Vec4:
		(*os) << "vec4(" << *(vec4*)root->value << ")";
		break;
	}
	(*os) << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Storages::Variable* root)
{
	tabulate();
	auto iter = find_if(storages->begin(), storages->end(), [root](pair<string, Instructions::Expressions::Storage*> el){
		return el.second == root; });
	(*os) << "Variable '"  << iter->first << "'" << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Storages::In* root)
{
	tabulate();
	auto iter = find_if(storages->begin(), storages->end(), [root](pair<string, Instructions::Expressions::Storage*> el){
		return el.second == root; });
	(*os) << "IN '" << iter->first << "'" << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Storages::Out* root)
{
	tabulate();
	auto iter = find_if(storages->begin(), storages->end(), [root](pair<string, Instructions::Expressions::Storage*> el){
		return el.second == root; });
	(*os) << "OUT '" << iter->first << "'" << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Operators::BinaryOperator* root)
{
	tabulate();
	(*os) << "Binary operator" << endl;
}
void TreeSerializer::visit(Instructions::Expressions::Operators::Binary::Plus* root)
{
	tabulate();
	(*os) << "+ operator" << endl;
	level++;
	tabulate();
	(*os) << " (left)" << endl;
	root->left->Accept(this);
	tabulate();
	(*os) << " (right)" << endl;
	root->right->Accept(this);
	level--;
}
void TreeSerializer::visit(Instructions::Expressions::Operators::Binary::Minus* root)
{
	tabulate();
	(*os) << "- operator" << endl;
	level++;
	tabulate();
	(*os) << " (left)" << endl;
	root->left->Accept(this);
	tabulate();
	(*os) << " (right)" << endl;
	root->right->Accept(this);
	level--;
}
void TreeSerializer::visit(Instructions::Expressions::Operators::Binary::Multiply* root)
{
	tabulate();
	(*os) << "* operator" << endl;
	level++;
	tabulate();
	(*os) << " (left)" << endl;
	root->left->Accept(this);
	tabulate();
	(*os) << " (right)" << endl;
	root->right->Accept(this);
	level--;
}
void TreeSerializer::visit(Instructions::Expressions::Operators::Binary::Divide* root)
{
	tabulate();
	(*os) << "/ operator" << endl;
	level++;
	tabulate();
	(*os) << " (left)" << endl;
	root->left->Accept(this);
	tabulate();
	(*os) << " (right)" << endl;
	root->right->Accept(this);
	level--;
}