#include "parse.h"

using namespace Blare;

ExampleParser::ExampleParser() {
	addRule(
		RULE_FUNC_DECL,
		{ ParseTerm(
			  { KEYWORD_FUNC, TOKEN_SYMBOL, '(', ')' },
			  [](shared_ptr<Token>& self, TokenList tokens) -> void {
				  printf(
					  "funcdecl: %s\n",
					  ((ValuedToken<std::string>*)(tokens[1].get()))->getData().c_str());
			  }),
			ParseTerm(
				{ KEYWORD_FUNC, TOKEN_SYMBOL, '(', RULE_PARAM_DECL, ')' },
				[](shared_ptr<Token>& self, TokenList tokens) -> void {
					printf(
						"funcdecl: %s\n",
						((ValuedToken<std::string>*)(tokens[1].get()))->getData().c_str());
				}) });
	addRule(
		RULE_PARAM_DECL,
		{ ParseTerm(
			  { RULE_VAR_DECL, ',', RULE_PARAM_DECL },
			  [](shared_ptr<Token>& self, TokenList tokens) -> void {
				  VarDeclToken* varDecl = (VarDeclToken*)tokens[0].get();

				  self = ParamDeclToken::make(std::make_shared<ParamDeclData>(varDecl->getData(), nullptr));
				  printf("paramdecl\n");
			  }),
			ParseTerm(
				{ RULE_VAR_DECL },
				[](shared_ptr<Token>& self, TokenList tokens) -> void {
					VarDeclToken* varDecl = (VarDeclToken*)tokens[0].get();

					self = ParamDeclToken::make(std::make_shared<ParamDeclData>(varDecl->getData(), nullptr));
					printf("paramdecl\n");
				}) });
	addRule(
		RULE_VAR_DECL,
		{ ParseTerm(
			{ KEYWORD_VAR, TOKEN_SYMBOL },
			[](shared_ptr<Token>& self, TokenList tokens) -> void {
				SymbolToken* symbol = (SymbolToken*)tokens[1].get();

				self = VarDeclToken::make({ symbol->getData() });
				printf("vardecl: %s\n", symbol->getData().c_str());
			}) });
}

ExampleParser::~ExampleParser() {
}
