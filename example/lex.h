#include <blare/lexer.h>

typedef enum {
	TOKEN_INT = SCHAR_MAX,
	TOKEN_UINT,
	TOKEN_LONG,
	TOKEN_ULONG,
	TOKEN_FLOAT,
	TOKEN_DOUBLE,
	TOKEN_SYMBOL,

	KEYWORD_FUNC,
	KEYWORD_VAR
} LexicalTokens;

class ExampleLexer : public Blare::Lexer {
public:
	ExampleLexer();
	virtual ~ExampleLexer();
};

using IntToken = Blare::ValuedToken<int32_t>;
using UIntToken = Blare::ValuedToken<uint32_t>;
using LongToken = Blare::ValuedToken<int64_t>;
using ULongToken = Blare::ValuedToken<uint64_t>;
using FloatToken = Blare::ValuedToken<float>;
using DoubleToken = Blare::ValuedToken<double>;
using SymbolToken = Blare::ValuedToken<std::string>;
