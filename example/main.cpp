#include <blare/lexer.h>

typedef enum {
	TOKEN_INT = SCHAR_MAX,
	TOKEN_UINT,
	TOKEN_FLOAT,
	TOKEN_DOUBLE,
	TOKEN_SYMBOL
} LexicalTokens;

template <const char* s>
Blare::LexRule::ThenProc funcTokenRule() {
	return [](std::smatch result) -> std::shared_ptr<Blare::Token> {
		return std::make_shared<Blare::Token>(s, 0);
	};
}

class ExampleLexer : public Blare::Lexer {
public:
	ExampleLexer() : Lexer() {
		auto initialState = std::make_shared<Blare::State>();
		int currentId = 0;
		registerState(STATE_INITIAL, std::weak_ptr<Blare::State>(initialState));
		initialState->registerRule(
			currentId++,
			R"([-]?[0-9]+)",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return Blare::ValuedToken<int>::make(TOKEN_INT, 0, atoi(result.str().c_str()));
			});
		initialState->registerRule(
			currentId++,
			R"([-]?[0-9]+[uU])",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return Blare::ValuedToken<unsigned int>::make(TOKEN_UINT, 0, strtoul(result.str().c_str(), nullptr, 0));
			});
		initialState->registerRule(
			currentId++,
			R"([-]?(?:(?:[0-9]+\.[0-9]+)|(?:\.[0-9]+)|(?:[0-9]+\.)|(?:[0-9]+))[fF])",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return Blare::ValuedToken<float>::make(TOKEN_FLOAT, 0, (float)atof(result.str().c_str()));
			});
		initialState->registerRule(
			currentId++,
			R"([-]?(?:(?:[0-9]+\.[0-9]+)|(?:\.[0-9]+)|(?:[0-9]+\.)|(?:[0-9]+)))",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return Blare::ValuedToken<double>::make(TOKEN_DOUBLE, 0, atof(result.str().c_str()));
			});
		initialState->registerRule(
			currentId++,
			R"([a-zA-Z_][a-zA-Z0-9_]*)",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return Blare::ValuedToken<std::string>::make(TOKEN_SYMBOL, 0, result.str());
			});
		initialState->registerRule(
			currentId++,
			R"(\()",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return std::make_shared<Blare::Token>('(', 0);
			});
		initialState->registerRule(
			currentId++,
			R"(\))",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return std::make_shared<Blare::Token>(')', 0);
			});
		initialState->registerRule(
			currentId++,
			R"(,)",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				return std::make_shared<Blare::Token>(',', 0);
			});
		initialState->registerRule(
			currentId++,
			R"([ \t\n\r]+)",
			[](std::smatch result) -> std::shared_ptr<Blare::Token> {
				auto token = std::make_shared<Blare::Token>(-1, 0);
				return token;
			});
	}
};

int main(int argc, char** argv) {
	std::shared_ptr<ExampleLexer> lexer = std::make_shared<ExampleLexer>();
	char input[4096 + 1] = { 0 };
	fgets(input, 4096, stdin);
	auto tokens = lexer->lex(input);
	for (auto i : tokens) {
		printf("Token: %d\n", i->getToken());
	}
	return 0;
}