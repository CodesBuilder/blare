#include <blare/parser.h>
#include "lexer.h"

class ExampleParser : public Blare::Parser {
public:
	ExampleParser();
	virtual ~ExampleParser();
};

typedef enum {
	RULE_TEST = -10000,
	RULE_TESTSUB
} RuleTokens;
