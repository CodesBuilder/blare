#include "parser.h"

using namespace Blare;

Blare::Parser::Parser() {
}

Blare::Parser::~Parser() {
}

void Blare::Parser::parse(const TokenList& tokens) {
	auto curToken = tokens.begin(), end = tokens.end();
	while (curToken != end) {
		for (auto curRule : parseRules) { // Match for each rules.
			// Make a copy for scanning.
			auto scanCurToken = curToken;

			try {
				scanCurToken = parseCore(scanCurToken, end, curRule.first);
			} catch (SyntaxError) {
				continue;
			}
			curToken = scanCurToken;
			goto succeed;
		}
		// If there's no any rule can be matched, throw a syntax error.
		throw SyntaxError(*curToken);
	succeed:;
	}
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
	TokenID token) {
	const auto& curRule = parseRules[token];
	while (curToken != end) {
		auto scanCurToken = curToken;		  // Make a copy for scanning.
		for (auto curTerm : curRule->terms) { // Match for each terms in the rule.
			scanCurToken = curToken;
			for (auto curRuleToken : curTerm) { // Match for each tokens in the term.
				if (scanCurToken == end)
					// Mismatch if reached the end.
					goto mismatched;
				// Check if current rule token represents a parse rule.
				if (curRuleToken < 0) {
					try {
						std::deque<ParseRule::ThenProc> thenTasks;
						scanCurToken = parseCore(scanCurToken, end, curRuleToken);
					} catch (SyntaxError) {
						goto mismatched;
					}
				} else {
					if (curRuleToken != (*scanCurToken)->getToken())
						goto mismatched;
					scanCurToken++; // Skip scanned token
				}
			}

			{
				TokenList ls;
				for (auto i = curToken; i != scanCurToken; i++)
					ls.push_back(*i);
				curRule->then(ls);

				curToken = scanCurToken;
				goto succeed;
			}
		mismatched:;
		}
		// Throw if no any rule was matched.
		throw SyntaxError(*scanCurToken);
	}

succeed:;
	return curToken;
}

void Blare::Parser::addRule(TokenID id, std::shared_ptr<ParseRule>& rule) {
	parseRules[id] = rule;
}

Blare::SyntaxError::SyntaxError(std::shared_ptr<Token> token) : runtime_error("Syntax error") {
	this->token = token;
}

Blare::SyntaxError::SyntaxError(std::shared_ptr<Token> token, std::string msg) : runtime_error(msg) {
	this->token = token;
}

std::shared_ptr<Token> Blare::SyntaxError::getToken() {
	return token;
}

Blare::SyntaxError::~SyntaxError() {
}

Blare::ParseRule::ParseRule(std::deque<std::deque<TokenID>> ls, ThenProc then) : terms(ls) {
	this->then = then;
}

Blare::ParseRule::~ParseRule() {
}

std::shared_ptr<ParseRule> Blare::ParseRule::make(std::deque<std::deque<TokenID>> ls, ThenProc then) {
	return std::make_shared<ParseRule>(ls, then);
}
