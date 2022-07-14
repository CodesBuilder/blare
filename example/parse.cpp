#include "parse.h"

#include <assert.h>

using namespace Blare;

template <typename T>
void sharedNullPtrFilter(T* p) {
	if (!p)
		return;
	delete p;
};

ExampleParser::ExampleParser() {
	const auto statementAction = [](shared_ptr<Token>& self, TokenList tokens) -> void {
		self = tokens[0];
		puts("Statement");
	};

	addRule(
		RULE_STATEMENTS,
		{ ParseTerm(
			  RULE_STATEMENT, RULE_STATEMENTS,
			  [](shared_ptr<Token>& self, TokenList tokens) -> void {
				  std::shared_ptr<StatementToken> statement = (std::shared_ptr<StatementToken>)tokens[0];
				  std::shared_ptr<StatementsToken> next = std::dynamic_pointer_cast<StatementsToken>(tokens[1]);

				  self = StatementsToken::make(
					  { std::shared_ptr<StatementToken>(statement),
						  std::shared_ptr<StatementsToken>(next) });
				  puts("Statements");
			  }),
			ParseTerm(
				RULE_STATEMENT,
				[](shared_ptr<Token>& self, TokenList tokens) -> void {
					std::shared_ptr<StatementToken> statement = (std::shared_ptr<StatementToken>)tokens[0];

					self = StatementsToken::make(
						{ std::shared_ptr<StatementToken>(statement),
							std::shared_ptr<StatementsToken>(nullptr, sharedNullPtrFilter<StatementsToken>) });
					puts("Statements");
				}) });
	addRule(
		RULE_STATEMENT,
		{ ParseTerm(RULE_FUNC_DECL, statementAction),
			ParseTerm(RULE_VAR_DECL, statementAction) });
	addRule(
		RULE_FUNC_DECL,
		{ ParseTerm(
			  KEYWORD_FUNC, TOKEN_SYMBOL, '(', ')',
			  [](shared_ptr<Token>& self, TokenList tokens) -> void {
				  std::shared_ptr<SymbolToken> symbol = std::dynamic_pointer_cast<SymbolToken>(tokens[1]);
				  self = FuncDeclToken::make(
					  { symbol->getData(),
						  std::shared_ptr<ParamDeclToken>(nullptr, sharedNullPtrFilter<ParamDeclToken>) });
				  printf(
					  "funcdecl: %s\n",
					  ((ValuedToken<std::string>*)(tokens[1].get()))->getData().c_str());
			  }),
			ParseTerm(
				KEYWORD_FUNC, TOKEN_SYMBOL, '(', RULE_PARAM_DECL, ')',
				[](shared_ptr<Token>& self, TokenList tokens) -> void {
					std::shared_ptr<SymbolToken> symbol = std::dynamic_pointer_cast<SymbolToken>(tokens[1]);
					std::shared_ptr<ParamDeclToken> paramDecl = std::dynamic_pointer_cast<ParamDeclToken>(tokens[3]);

					self = FuncDeclToken::make(
						{ symbol->getData(),
							std::shared_ptr<ParamDeclToken>(paramDecl) });
					printf(
						"funcdecl: %s\n",
						((ValuedToken<std::string>*)(tokens[1].get()))->getData().c_str());
				}) });
	addRule(
		RULE_PARAM_DECL,
		{ ParseTerm(
			  RULE_VAR_DECL, ',', RULE_PARAM_DECL,
			  [](shared_ptr<Token>& self, TokenList tokens) -> void {
				  std::shared_ptr<VarDeclToken> varDecl = std::dynamic_pointer_cast<VarDeclToken>(tokens[0]);
				  std::shared_ptr<ParamDeclToken> paramDecl = std::dynamic_pointer_cast<ParamDeclToken>(tokens[2]);

				  self = ParamDeclToken::make(
					  { varDecl->getData(),
						  std::shared_ptr<ParamDeclToken>(paramDecl) });
				  printf("paramdecl\n");
			  }),
			ParseTerm(
				RULE_VAR_DECL,
				[](shared_ptr<Token>& self, TokenList tokens) -> void {
					std::shared_ptr<VarDeclToken> varDecl = std::dynamic_pointer_cast<VarDeclToken>(tokens[0]);

					self = ParamDeclToken::make(
						{ varDecl->getData(),
							std::shared_ptr<ParamDeclToken>(nullptr, sharedNullPtrFilter<ParamDeclToken>) });
					printf("paramdecl\n");
				}) });
	addRule(
		RULE_VAR_DECL,
		{ ParseTerm(
			KEYWORD_VAR, TOKEN_SYMBOL,
			[](shared_ptr<Token>& self, TokenList tokens) -> void {
				SymbolToken* symbol = (SymbolToken*)tokens[1].get();

				self = VarDeclToken::make({ symbol->getData() });
				printf("vardecl: %s\n", symbol->getData().c_str());
			}) });
}

ExampleParser::~ExampleParser() {
}
