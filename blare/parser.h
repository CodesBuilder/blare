#ifndef __BLARE_PARSER_H__
#define __BLARE_PARSER_H__

#include "lexer.h"

namespace Blare {
	class SyntaxError : std::runtime_error {
	protected:
		size_t line;

	public:
		explicit SyntaxError(size_t line);
		virtual ~SyntaxError();
		virtual size_t getLine();
	};

	class ParseRule {
	public:
		using ThenProc = std::function<void(TokenID tokens)>;
		ParseRule& operator=(ParseRule&) = delete;

	protected:
		TokenID tokens;
		ThenProc then;
	};

	class Parser {
	protected:
		std::map<TokenID, ParseRule> parseRules;

	public:
		Parser();
		virtual ~Parser();

		void parse(
			const Util::ArrayList<std::shared_ptr<Token>>& tokens);
	};
}

#endif