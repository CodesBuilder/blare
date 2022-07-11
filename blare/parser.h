#ifndef __BLARE_PARSER_H__
#define __BLARE_PARSER_H__

#include "lexer.h"

namespace Blare {
	class SyntaxError : public std::runtime_error {
	protected:
		std::shared_ptr<Token> token;

	public:
		explicit SyntaxError(std::shared_ptr<Token> token);
		explicit SyntaxError(std::shared_ptr<Token> token, std::string msg);
		virtual std::shared_ptr<Token> getToken();
		virtual ~SyntaxError();
	};

	struct ParseTerm final {
		using ActionProc = std::function<void(std::shared_ptr<Token>& self, TokenList& tokens)>;
		std::deque<TokenID> tokens;
		ActionProc action;

		ParseTerm(std::deque<TokenID> tokens, ActionProc action);
		~ParseTerm();
		inline ParseTerm& operator=(const ParseTerm& x) {
			tokens = x.tokens;
			action = x.action;
			return *this;
		}
	};

	struct ParseRule final {
		std::deque<ParseTerm> terms;
		ParseRule();
		ParseRule(const std::deque<ParseTerm>& terms);
		~ParseRule();
		inline ParseRule& operator=(const ParseRule& x) {
			terms = x.terms;
			return *this;
		}
	};

	class Parser {
	protected:
		std::map<TokenID, ParseRule, std::less<TokenID>> parseRules;

		virtual TokenList::const_iterator parseCore(
			TokenList::const_iterator begin,
			TokenList::const_iterator end,
			TokenID token,
			shared_ptr<Token>& dest);
		virtual void addRule(TokenID id, ParseRule rule);
		virtual void addRule(TokenID id, std::initializer_list<ParseTerm> terms) {
			addRule(id, ParseRule(terms));
		}

	public:
		Parser();
		virtual ~Parser();

		virtual std::shared_ptr<Token> parse(const TokenList& tokens);
	};
}

#endif
