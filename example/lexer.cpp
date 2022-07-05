#include "lexer.h"

ExampleLexer::ExampleLexer() : Lexer() {
	auto initialState = std::make_shared<Blare::State>();
	auto stringState = std::make_shared<Blare::State>();
	auto stringEscapeState = std::make_shared<Blare::State>();
	auto commentState = std::make_shared<Blare::State>();

	int currentId = 0;
	registerState(STATE_INITIAL, std::weak_ptr<Blare::State>(initialState));

	initialState->addRule(
		R"([-]?[0-9]+)",
		[](std::smatch result, auto& dest) -> bool {
			dest = Blare::ValuedToken<int32_t>::make(TOKEN_INT, 0, atoi(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([-]?[0-9]+[uU])",
		[](std::smatch result, auto& dest) -> bool {
			dest = Blare::ValuedToken<uint32_t>::make(TOKEN_UINT, 0, strtoul(result.str().c_str(), nullptr, 0));
			return true;
		});
	initialState->addRule(
		R"([-]?[0-9]+[lL])",
		[](std::smatch result, auto& dest) -> bool {
			dest = Blare::ValuedToken<int64_t>::make(TOKEN_INT, 0, atoi(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([-]?[0-9]+(?:(?:[uU][lL])|(?:[lL][uU])))",
		[](std::smatch result, auto& dest) -> bool {
			dest = Blare::ValuedToken<int64_t>::make(TOKEN_UINT, 0, strtoul(result.str().c_str(), nullptr, 0));
			return true;
		});
	initialState->addRule(
		R"([-]?(?:(?:[0-9]+\.[0-9]+)|(?:\.[0-9]+)|(?:[0-9]+\.)|(?:[0-9]+))[fF])",
		[](std::smatch result, auto& dest) -> bool {
			dest = Blare::ValuedToken<float>::make(TOKEN_FLOAT, 0, (float)atof(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([-]?(?:(?:[0-9]+\.[0-9]+)|(?:\.[0-9]+)|(?:[0-9]+\.)|(?:[0-9]+)))",
		[](std::smatch result, auto& dest) -> bool {
			dest = Blare::ValuedToken<double>::make(TOKEN_DOUBLE, 0, atof(result.str().c_str()));
			return true;
		});
	initialState->addRule(
		R"([a-zA-Z_][a-zA-Z0-9_]*)",
		[](std::smatch result, auto& dest) -> bool {
			dest = Blare::ValuedToken<std::string>::make(TOKEN_SYMBOL, 0, result.str());
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

	initialState->addRule(R"([ \t\n\r]+)", Blare::TOKEN_INVALID); // Ignored
}

ExampleLexer::~ExampleLexer() {
}
