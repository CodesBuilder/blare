#include "parser.h"

using namespace Blare;

Blare::Parser::Parser() {
}

Blare::Parser::~Parser() {
}

void Blare::Parser::addRule(TokenID id, ParseRule rule) { parseRules[id] = rule; }

shared_ptr<Token> Blare::Parser::parse(const TokenList& tokens) {
	auto curToken = tokens.begin(), end = tokens.end();
	shared_ptr<Token> token(nullptr);

	// The first rule will be assumed as the root rule.
	curToken = parseCore(curToken, end, parseRules.begin()->first, token);

	// If tokens were not complete matched, throw a syntax error.
	if (curToken != end)
		throw SyntaxError(*curToken);

	return token;
}

/**
 * @brief Core function for parsing.
 * @param begin Beginning of the parse range.
 * @param end End of the parse range.
 * @param token Token of parse rule.
 * @return Next of the last parsed token.
 */
TokenList::const_iterator Blare::Parser::parseCore(
	TokenList::const_iterator curToken,
	TokenList::const_iterator end,
	TokenID token,
	shared_ptr<Token>& dest) {
	const auto& curRule = parseRules[token];

	// Match for each terms in the rule.
	for (auto curTerm : curRule.terms) {
		auto scanCur = curToken;  // Iterator of current scanning token

		TokenList tokenList;
		// Match for each tokens in the term
		for (auto curRuleToken : curTerm.tokens) {
			if (scanCur == end)	 // Mismatch if reached the end
				goto mismatched;

			// Current token
			const auto scanCurToken = scanCur->get()->getToken();

			if (isLexicalToken(curRuleToken)) {
				if (curRuleToken != scanCurToken)
					goto mismatched;

				tokenList.push_back(*scanCur);
				scanCur++;	// Skip scanned token
			} else {
				// The token represents a node of the rule.
				shared_ptr<Token> selfToken(nullptr);

				try {
					scanCur = parseCore(scanCur, end, curRuleToken, selfToken);
				} catch (SyntaxError) {
					goto mismatched;
				}

				selfToken->setLine(curToken->get()->getLine());
				tokenList.push_back(selfToken);
			}
		}
		curTerm.action(dest, tokenList);

		return scanCur;
	mismatched:;
	}
	// Throw a syntax error if no any rule was matched.
	throw SyntaxError(*curToken);
}

Blare::SyntaxError::SyntaxError(shared_ptr<Token> token) : runtime_error("Syntax error") {
	this->token = token;
}
Blare::SyntaxError::SyntaxError(shared_ptr<Token> token, std::string msg) : runtime_error(msg) {
	this->token = token;
}

shared_ptr<Token> Blare::SyntaxError::getToken() { return token; }
Blare::SyntaxError::~SyntaxError() {}

Blare::ParseTerm::ParseTerm(std::deque<TokenID> tokens, ActionProc action) {
	this->tokens = tokens;
	this->action = action;
}
Blare::ParseTerm::~ParseTerm() {}

Blare::ParseRule::ParseRule() {
}
Blare::ParseRule::ParseRule(const std::deque<ParseTerm>& terms) {
	this->terms = terms;
}
Blare::ParseRule::~ParseRule() {}
