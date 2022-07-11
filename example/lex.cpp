#include "lex.h"

ExampleLexer::ExampleLexer() : Lexer() {
	auto initialState = std::make_shared<Blare::State>();
	auto stringState = std::make_shared<Blare::State>();
	auto stringEscapeState = std::make_shared<Blare::State>();
	auto commentState = std::make_shared<Blare::State>();

	int currentId = 0;
	registerState(STATE_INITIAL, std::weak_ptr<Blare::State>(initialState));

	initialState->addRule(R"(func)", KEYWORD_FUNC);
	initialState->addRule(R"(var)", KEYWORD_VAR);

	initialState->addRule(
		R"([-]?[0-9]+)",
		[](std::smatch result, auto& dest) -> bool {
			dest = IntToken::make(TOKEN_INT, atoi(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([-]?[0-9]+[uU])",
		[](std::smatch result, auto& dest) -> bool {
			dest = UIntToken::make(TOKEN_UINT, strtoul(result.str().c_str(), nullptr, 0));
			return true;
		});
	initialState->addRule(
		R"([-]?[0-9]+[lL])",
		[](std::smatch result, auto& dest) -> bool {
			dest = LongToken::make(TOKEN_LONG, atoi(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([-]?[0-9]+(?:(?:[uU][lL])|(?:[lL][uU])))",
		[](std::smatch result, auto& dest) -> bool {
			dest = ULongToken::make(TOKEN_ULONG, strtoul(result.str().c_str(), nullptr, 0));
			return true;
		});
	initialState->addRule(
		R"([-]?(?:(?:[0-9]+\.[0-9]+)|(?:\.[0-9]+)|(?:[0-9]+\.)|(?:[0-9]+))[fF])",
		[](std::smatch result, auto& dest) -> bool {
			dest = FloatToken::make(TOKEN_FLOAT, (float)atof(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([-]?(?:(?:[0-9]+\.[0-9]+)|(?:\.[0-9]+)|(?:[0-9]+\.)|(?:[0-9]+)))",
		[](std::smatch result, auto& dest) -> bool {
			dest = DoubleToken::make(TOKEN_DOUBLE, atof(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([a-zA-Z_][a-zA-Z0-9_]*)",
		[](std::smatch result, auto& dest) -> bool {
			dest = SymbolToken::make(TOKEN_SYMBOL, result.str());
			return true;
		});

	initialState->addRule(R"(@)", '@');
	initialState->addRule(R"(#)", '#');
	initialState->addRule(R"(\$)", '$');
	initialState->addRule(R"(!)", '!');
	initialState->addRule(R"(\+)", '+');
	initialState->addRule(R"(-)", '-');
	initialState->addRule(R"(\*)", '*');
	initialState->addRule(R"(\/)", '/');
	initialState->addRule(R"(%)", '%');
	initialState->addRule(R"(&)", '&');
	initialState->addRule(R"(\|)", '|');
	initialState->addRule(R"(\^)", '^');
	initialState->addRule(R"(>)", '>');
	initialState->addRule(R"(<)", '<');
	initialState->addRule(R"(=)", '=');
	initialState->addRule(R"(~)", '~');
	initialState->addRule(R"(\?)", '?');

	initialState->addRule(R"(\()", '(');
	initialState->addRule(R"(\))", ')');
	initialState->addRule(R"(\[)", '[');
	initialState->addRule(R"(\])", ']');

	initialState->addRule(R"(\.)", '.');
	initialState->addRule(R"(,)", ',');
	initialState->addRule(R"(;)", ';');

	initialState->addRule(R"([ \t\n\r]+)", Blare::TOKEN_INVALID);  // Ignored
}

ExampleLexer::~ExampleLexer() {
}
