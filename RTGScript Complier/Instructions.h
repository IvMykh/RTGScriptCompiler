#pragma once
#include <iostream>
#include <string>
#include <vector>


typedef signed __int8						int8;
typedef unsigned __int8						uint8;
typedef signed __int16						int16;
typedef unsigned __int16					uint16;
typedef signed __int32						int32;
typedef unsigned __int32					uint32;
typedef signed __int64						int64;
typedef unsigned __int64					uint64;
typedef float								float32;


struct vec2
{
	float32 x, y;

	friend std::ostream& operator << (std::ostream& stream, const vec2& vec)
	{
		stream << vec.x << "," << vec.y;
		return stream;
	}
};
struct vec3
{
	float32 x, y, z;

	friend std::ostream& operator << (std::ostream& stream, const vec3& vec)
	{
		stream << vec.x << "," << vec.y << "," << vec.z;
		return stream;
	}
};
struct vec4
{
	float32 x, y, z, w;

	friend std::ostream& operator << (std::ostream& stream, const vec4& vec)
	{
		stream << vec.x << "," << vec.y << "," << vec.z << "," << vec.w;
		return stream;
	}
};
struct ivec2
{
	int32 x, y;

	friend std::ostream& operator << (std::ostream& stream, const ivec2& vec)
	{
		stream << vec.x << "," << vec.y;
		return stream;
	}
};
struct ivec3
{
	int32 x, y, z;

	friend std::ostream& operator << (std::ostream& stream, const ivec3& vec)
	{
		stream << vec.x << "," << vec.y << "," << vec.z;
		return stream;
	}
};
struct ivec4
{
	int32 x, y, z, w;

	friend std::ostream& operator << (std::ostream& stream, const ivec4& vec)
	{
		stream << vec.x << "," << vec.y << "," << vec.z << "," << vec.w;
		return stream;
	}
};
struct mat2
{
	float32 data[4];
};
struct mat3
{
	float32 data[9];
};
struct mat4
{
	float32 data[16];
};


struct Translator;
class Visitor;


struct Instruction
{
	enum class Type
	{
		Block,
		Definition,
		Expression,
		Statement,
		Empty
	};

	virtual ~Instruction() = default;

	virtual Instruction::Type				GetInstructionType() const
	{
		return Type::Empty;
	}
	virtual void							Accept(Translator* translator);
	virtual void							Accept(Visitor* visitor);
};
namespace Instructions
{
	struct Block;
	struct Definition;
	struct Expression;
	namespace Expressions
	{
		struct Storage;
		struct Operator;
	}

	struct Block :
		public Instruction
	{
		std::vector<Instruction*>				content;

		virtual Instruction::Type				GetInstructionType() const
		{
			return Instruction::Type::Block;
		}
		virtual void							Accept(Translator* translator);
		virtual void							Accept(Visitor* visitor);

		virtual ~Block();
	};
	struct Definition :
		public Instruction
	{
		Expressions::Storage*					storage = nullptr;
		Expression*								initializer = nullptr;

		inline Definition() = default;
		inline Definition(Expressions::Storage* storage_, Expression* initializer_) :
			storage(storage_), initializer(initializer_)
		{
		}

		virtual Instruction::Type				GetInstructionType() const
		{
			return Instruction::Type::Definition;
		}
		virtual void							Accept(Translator* translator);
		virtual void							Accept(Visitor* visitor);

		virtual ~Definition();
	};
	struct Expression :
		public Instruction
	{
		enum class Type
		{
			Storage,
			Operator
		};

		virtual Expression::Type				GetExpressionType() const = 0;
		virtual Instruction::Type				GetInstructionType() const
		{
			return Instruction::Type::Expression;
		}
		virtual void							Accept(Translator* translator);
		virtual void							Accept(Visitor* visitor);
	};

	namespace Expressions
	{
		struct Storage :
			public Expression
		{
			struct External
			{
				void*							location = nullptr;

				inline External() = default;
				inline External(void* location_) :
					location(location_)
				{
				}
			};

			enum class Type
			{
				Bool, Int, Float,
				Vec2, IVec2,
				Vec3, IVec3,
				Vec4, IVec4,
				Mat2, Mat3, Mat4,
				Texture1D, Texture2D, Texture3D, TextureCube,
				__size__
			};

			static std::string TypeNames[(unsigned int)Type::__size__];

			Type								type;

			inline Storage() = default;
			inline Storage(Type& type_) :
				type(type_)
			{
			}

			inline Storage::Type				GetStorageType() const
			{
				return type;
			}
			virtual Expression::Type			GetExpressionType() const
			{
				return Expression::Type::Storage;
			}
			virtual void						Accept(Translator* translator);
			virtual void						Accept(Visitor* visitor);
		};
		
		struct Operator :
			public Expression
		{
			virtual Expression::Type			GetExpressionType() const
			{
				return Expression::Type::Operator;
			}
			virtual void						Accept(Translator* translator);
			virtual void						Accept(Visitor* visitor);
		};

		namespace Storages
		{
			struct Literal :
				public Storage
			{
				void*						value = nullptr;

				inline Literal() = default;
				inline Literal(Type type_, void* value_) :
					Storage(type_),
					value(value_)
				{
				}

				virtual void				Accept(Translator* translator);
				virtual void				Accept(Visitor* visitor);

				virtual ~Literal();
			};
			struct Variable :
				public Storage
			{
				inline Variable() = default;
				inline Variable(Storage::Type type_) :
					Storage(type_)
				{
				}

				virtual void				Accept(Translator* translator);
				virtual void				Accept(Visitor* visitor);
			};
			struct Out :
				public Variable,
				public Storage::External
			{
				inline Out() = default;
				inline Out(Storage::Type type_, void* location_) :
					Variable(type_),
					External(location_)
				{
				}

				virtual void				Accept(Translator* translator);
				virtual void				Accept(Visitor* visitor);
			};
			struct In :
				public Storage,
				public Storage::External
			{
				inline In() = default;
				inline In(Storage::Type type_, void* location_) :
					Storage(type_),
					External(location_)
				{
				}

				virtual void				Accept(Translator* translator);
				virtual void				Accept(Visitor* visitor);
			};
		}
		namespace Operators
		{
			struct BinaryOperator :
				public Operator
			{
				Expression*				left = nullptr;
				Expression*				right = nullptr;

				inline BinaryOperator() = default;
				inline BinaryOperator(Expression* left_, Expression* right_) :
					Operator(),
					left(left_),
					right(right_)
				{
				}

				virtual int				priority() { return 0; }

				virtual void			Accept(Translator* translator);
				virtual void			Accept(Visitor* visitor);

				virtual ~BinaryOperator();
			};
			namespace Binary
			{
				struct Assignment :
					public Operator
				{
					Storages::Variable*		left = nullptr;
					Expression*				right = nullptr;

					inline Assignment() = default;
					inline Assignment(Storages::Variable* left_, Expression* right_) :
						Operator(),
						left(left_),
						right(right_)
					{
					}

					virtual void			Accept(Translator* translator);
					virtual void			Accept(Visitor* visitor);

					virtual ~Assignment();
				};
				struct Plus :
					public BinaryOperator
				{
					inline Plus() = default;
					inline Plus(Expression* left_, Expression* right_) :
						BinaryOperator(left_, right_)
					{
					}

					virtual int				priority() { return 1; }

					virtual void			Accept(Translator* translator);
					virtual void			Accept(Visitor* visitor);
				};
				struct Minus :
					public BinaryOperator
				{
					inline Minus() = default;
					inline Minus(Expression* left_, Expression* right_) :
						BinaryOperator(left_, right_)
					{
					}

					virtual int				priority() { return 1; }

					virtual void			Accept(Translator* translator);
					virtual void			Accept(Visitor* visitor);
				};
				struct Multiply :
					public BinaryOperator
				{
					inline Multiply() = default;
					inline Multiply(Expression* left_, Expression* right_) :
						BinaryOperator(left_, right_)
					{
					}

					virtual int				priority() { return 2; }

					virtual void			Accept(Translator* translator);
					virtual void			Accept(Visitor* visitor);
				};
				struct Divide :
					public BinaryOperator
				{
					inline Divide() = default;
					inline Divide(Expression* left_, Expression* right_) :
						BinaryOperator(left_, right_)
					{
					}

					virtual int				priority() { return 2; }

					virtual void			Accept(Translator* translator);
					virtual void			Accept(Visitor* visitor);
				};
			}
		}
	}
}




