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

	struct ParseRule {
		using ThenProc = std::function<void(TokenList& tokens)>;
		ParseRule& operator=(ParseRule&) = delete;
		std::deque<std::deque<TokenID>> terms;
		ThenProc then;

		ParseRule(std::deque<std::deque<TokenID>> ls, ThenProc then);
		virtual ~ParseRule();

		static std::shared_ptr<ParseRule> make(std::deque<std::deque<TokenID>> ls, ThenProc then);
	};

	class Parser {
	protected:
		std::map<TokenID, std::shared_ptr<ParseRule>, std::less<TokenID>> parseRules;

		virtual TokenList::const_iterator parseCore(
			TokenList::const_iterator begin,
			TokenList::const_iterator end,
			TokenID token);
		virtual void addRule(TokenID id, std::shared_ptr<ParseRule>& rule);
	public:
		Parser();
		virtual ~Parser();

		virtual void parse(const TokenList& tokens);
	};
}

#endif
