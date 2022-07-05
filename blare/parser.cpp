#include "parser.h"

using namespace Blare;

Blare::Parser::Parser() {
}

Blare::Parser::~Parser() {
}

void Blare::Parser::parse(const TokenList& tokens) {
	auto curToken = tokens.begin(), end = tokens.end();
	while (curToken!=end) {
		for (auto curRule : parseRules) { // Match for each rules.
			// Make a copy for scanning.
			auto scanCurToken = curToken;

			try {
				scanCurToken=parseRecursion(scanCurToken, end, curRule.first);
			} catch (SyntaxError) {
				continue;
			}
			curToken = scanCurToken;
			goto succeed;
		}
		// There's no any rule can be matched, throw a syntax error exception.
		throw SyntaxError("Syntax error");
	succeed:;
	}
}

/**
 * @brief Subroutine for recursive parsing.
 * @param begin Begin of the parse range.
 * @param end End of the parse range.
 * @param token Token of parse rule.
 * @return Next of the last parsed token.
 */
TokenList::const_iterator Blare::Parser::parseRecursion(
	TokenList::const_iterator curToken,
	TokenList::const_iterator end,
	TokenID token) {
	auto curRule = parseRules[token];
	while (curToken != end) {
		// Make a copy for scanning.
		auto scanCurToken = curToken;

		for (auto curTerm : curRule->terms) {	// Match for each terms in the rule.
			for (auto curRuleToken : curTerm) { // Match for each tokens in the term.
				if (scanCurToken == end)
					// Mismatch if reached the end.
					goto mismatched;

				// Check if current rule token represents a parse rule.
				if (curRuleToken < 0) {
					try {
						scanCurToken = parseRecursion(scanCurToken, end, curRuleToken);
					} catch (SyntaxError) {
						goto mismatched;
					}
				} else {
					if (curRuleToken != (*scanCurToken)->getToken())
						// Current token and current token are mismatched.
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
		throw SyntaxError();
	succeed:;
	}
	return curToken;
}

void Blare::Parser::addRule(TokenID id, std::shared_ptr<ParseRule>& rule) {
	parseRules[id] = rule;
}

Blare::SyntaxError::SyntaxError() : runtime_error("") {
}

Blare::SyntaxError::SyntaxError(std::string msg) : runtime_error(msg) {
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
