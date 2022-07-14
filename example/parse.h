#include <blare/parser.h>

#include "lex.h"

class ExampleParser : public Blare::Parser {
public:
	ExampleParser();
	virtual ~ExampleParser();
};

typedef enum {
	RULE_STATEMENTS = -10000,  // Statements
	RULE_STATEMENT,			   // Any statement

	RULE_FUNC_DECL,	  // Function declaration
	RULE_PARAM_DECL,  // Parameter declaration

	RULE_VAR_DECL  // Variable declaration
} RuleTokens;

struct StatementsData;
struct FuncDeclData;
struct VarDeclData;
struct ParamDeclData;

using StatementsToken = Blare::ValuedToken<StatementsData>;
using StatementToken = Blare::Token;
using FuncDeclToken = Blare::ValuedToken<FuncDeclData>;
using ParamDeclToken = Blare::ValuedToken<ParamDeclData>;
using VarDeclToken = Blare::ValuedToken<VarDeclData>;

struct StatementsData final {
	std::shared_ptr<StatementToken> child;
	std::shared_ptr<StatementsToken> next;
};

struct FuncDeclData final {
	std::string name;
	std::shared_ptr<ParamDeclToken> params;
};

struct VarDeclData final {
	std::string name;
};

struct ParamDeclData final {
	VarDeclData varDecl;
	std::shared_ptr<ParamDeclToken> next;
};
