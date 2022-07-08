#include "parse.h"

ExampleParser::ExampleParser() {
	addRule(
		RULE_FUNC_DECL,
		Blare::ParseRule::make(
			{ { KEYWORD_FUNC, TOKEN_SYMBOL, '(', ')' },
				{ KEYWORD_FUNC, TOKEN_SYMBOL, '(', RULE_PARAM_DECL, ')' } },
			[](Blare::TokenList tokens) -> void {
				printf(
					"funcdecl: %s\n",
					((Blare::ValuedToken<std::string>*)(tokens[1].get()))->getData().c_str());
			}));
	addRule(
		RULE_PARAM_DECL,
		Blare::ParseRule::make(
			{ { RULE_VAR_DECL, ',', RULE_PARAM_DECL } ,
			{ RULE_VAR_DECL } },
			[](Blare::TokenList tokens) -> void {
				printf("paramdecl\n");
			}));
	addRule(
		RULE_VAR_DECL,
		Blare::ParseRule::make(
			{ { KEYWORD_VAR, TOKEN_SYMBOL } },
			[](Blare::TokenList tokens) -> void {
				printf(
					"var: %s\n",
					((Blare::ValuedToken<std::string>*)(tokens[1].get()))->getData().c_str());
			}));
}

ExampleParser::~ExampleParser() {
}
