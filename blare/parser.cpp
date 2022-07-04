#include "parser.h"

using namespace Blare;

Blare::Parser::Parser() {
}

Blare::Parser::~Parser() {
}

void Blare::Parser::parse(const TokenList& tokens) {
	auto begin = tokens.begin(), end = tokens.end();
	for (auto curToken = begin; curToken != end; curToken++) {
		// Scan for each rules.
		for (auto curRule : parseRules) {
			auto scanCurToken = curToken;
			// Match for each tokens in the rule.
			for (auto curRuleToken : curRule.second->tokens) {
				if (scanCurToken == end)
					goto mismatched; // Mismatch if reached the end.

				// Check if the token in the rule represents a parse rule.
				if (curRuleToken < 0) {
					try {
						scanCurToken = parseRecursion(scanCurToken, end, curRuleToken);
					} catch (SyntaxError) {
						break;
					}
				} else {
					// Check if current token matches the token in the rule.
					if (curRuleToken != (*scanCurToken)->getToken())
						goto mismatched;
					scanCurToken++; // Skip scanned token if matched
				}
			}
			{
				TokenList ls;
				auto i = curToken;
				while (i != end) {
					ls.push_back(*i);
					if (i == scanCurToken)
						break;
					i++;
				}
				curRule.second->then(ls);
			}
			// Reserved for increment in the loop end
			curToken = std::prev(scanCurToken);
			goto succeed;
		mismatched:;
		}
		// No rule matched.
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
	TokenList::const_iterator begin,
	TokenList::const_iterator end,
	TokenID token) {
	auto curRule = parseRules[token];
	while (begin != end) {
		auto scanCurToken = begin;
		// Match for each tokens in the rule.
		for (auto curRuleToken : curRule->tokens) {
			if (scanCurToken == end)
				goto mismatched; // Mismatch if reached the end.
			if (curRuleToken < 0) {
				try {
					// Check if the token in the rule represents a parse rule.
					scanCurToken = parseRecursion(scanCurToken, end, curRuleToken);
				} catch (SyntaxError) {
					break;
				}
			}
			// Check if current token matches the token in the rule.
			if (curRuleToken != (*scanCurToken)->getToken())
				goto mismatched;
			scanCurToken++; // Skip scanned token if matched
		}
		{
			TokenList ls;
			auto i = scanCurToken;
			while (i != end) {
				ls.push_back(*i);
				if (i == scanCurToken)
					break;
				i++;
			}
			curRule->then(ls);
		}
		begin = scanCurToken;
	}
	return begin;
mismatched:
	throw SyntaxError();
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

Blare::ParseRule::ParseRule(std::initializer_list<TokenID> ls, ThenProc then) : tokens(ls) {
	this->then = then;
}

Blare::ParseRule::~ParseRule() {
}

std::shared_ptr<ParseRule> Blare::ParseRule::make(std::initializer_list<TokenID> ls, ThenProc then) {
	return std::make_shared<ParseRule>(ls, then);
}
