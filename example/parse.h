#include <blare/parser.h>

#include "lex.h"

class ExampleParser : public Blare::Parser {
public:
	ExampleParser();
	virtual ~ExampleParser();
};

typedef enum {
	RULE_STATEMENT = -10000,

	RULE_FUNC_DECL,	  // Function declaration
	RULE_PARAM_DECL,  // Parameter declaration

	RULE_VAR_DECL  // Variable declaration
} RuleTokens;

struct VarDeclData;
struct ParamDeclData;

struct VarDeclData final {
	std::string name;
};

struct ParamDeclData final {
	VarDeclData varDecl;
	std::shared_ptr<ParamDeclData> next;

	ParamDeclData& operator=(const ParamDeclData&) = delete;

	inline ParamDeclData(VarDeclData& varDecl, std::shared_ptr<ParamDeclData> next) {
		this->varDecl = varDecl;
		this->next = next;
	}
	inline ParamDeclData(VarDeclData& varDecl) {
		this->varDecl = varDecl;
		this->next = std::shared_ptr<ParamDeclData>(nullptr);
	}
};

using ParamDeclToken = Blare::ValuedToken<std::shared_ptr<ParamDeclData>>;
using VarDeclToken = Blare::ValuedToken<VarDeclData>;
