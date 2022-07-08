#include "lex.h"
#include <blare/parser.h>

class ExampleParser : public Blare::Parser {
public:
	ExampleParser();
	virtual ~ExampleParser();
};

typedef enum {
	RULE_SUPERCMD = -10000,

	RULE_FUNC_DECL,	 // Function declaration
	RULE_PARAM_DECL, // Parameter declaration

	RULE_VAR_DECL // Variable declaration
} RuleTokens;
