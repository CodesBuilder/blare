#include <blare/lexer.h>

typedef enum {
	TOKEN_INT = SCHAR_MAX,
	TOKEN_UINT,
	TOKEN_FLOAT,
	TOKEN_DOUBLE,
	TOKEN_SYMBOL
} LexicalTokens;

class ExampleLexer : public Blare::Lexer {
public:
	ExampleLexer();
	virtual ~ExampleLexer();
};
