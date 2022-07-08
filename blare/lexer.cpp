#include "lexer.h"

using namespace Blare;

/**
 * @brief Constructor of class `Token'.
 * @param token Value of current token.
 * @param line Line where the token at.
 */
Blare::Token::Token(TokenID token, size_t line) {
	this->token = token;
	this->line = line;
}
Blare::Token::~Token() {}

/**
 * @brief Get value of the token.
 * @return Value of the token.
 */
TokenID Blare::Token::getToken() {
	return token;
}

void Blare::Token::setToken(TokenID token) {
	this->token = token;
}

/**
 * @brief Get the line where the token at.
 * @return Line where the token at.
 */
size_t Blare::Token::getLine() {
	return line;
}

void Blare::Token::setLine(size_t line) {
	this->line = line;
}

Blare::Lexer::Lexer() {
	currentState = STATE_INITIAL;
}

Blare::Lexer::~Lexer() {}

/**
 * @brief Register a state into the state.
 * @param id ID of new state.
 * @param state State to register.
 */
void Blare::Lexer::registerState(StateID id, std::weak_ptr<State> state) {
	if (state.expired())
		throw std::runtime_error("Source where the weak_ptr referenced has been expired");
	states[id] = state.lock();
}

/**
 * @brief Unregister a state.
 * @param id ID of state to unregister.
 */
void Blare::Lexer::unregisterState(StateID id) {
	states.erase(id);
}

/**
 * @brief Analyse a string with the lexical analyser.
 * @param src String to analyse.
 * @exception LexicalError Throws if any lexical error has occurred.
 * @return Generated token.
 */
TokenList Blare::Lexer::lex(std::string src) throw (LexicalError) {
	TokenList tokens;
	std::shared_ptr<Token> token = nullptr;
	for (size_t i = 0, line = 0; i < src.size();) {
		size_t end;

		// Try to match contents as more as possible.
		for (end = src.size(); end > i; end--) {
			// Sliced string to match.
			auto matchStr = src.substr(i, end - i);

			if (states[currentState]->match(matchStr, token)) {
				// Check if the rule hinted the analyser to discard current token
				if (token->getToken() != TOKEN_INVALID) {
					token->setLine(line);
					tokens.push_back(token);
				}

				// Increase line automatically.
				line += std::count(matchStr.begin(), matchStr.end(), '\n');
				token = nullptr;
				goto matched;
			}
			// Keep current token and skip scanned area if the rule returned false but created the token
			if (token)
				goto matched;
		}

		throw LexicalError(line);

	matched:
		i = end; // Skip scanned area.
	}

	return tokens;
}

/**
 * @brief Constructor of class `LexRule'.
 * @param rule Regular expression for matching.
 * @param then What to execute if matched.
 */
Blare::LexRule::LexRule(std::regex rule, ThenProc then) {
	this->rule = rule, this->then = then;
}

Blare::LexRule::~LexRule() {}

/**
 * @brief Match a string, execute `then' if the string was matched.
 * @param str String to match.
 * @return Generated token if matched, nullptr otherwise.
 */
bool Blare::LexRule::match(std::string str, std::shared_ptr<Token>& dest) {
	std::smatch result;
	if (!std::regex_match(str, result, rule))
		return false;
	return then(result,dest);
}

Blare::State::~State() {}

/**
 * @brief Match a string for each rules in the state till anyone was matched or matched nothing.
 * @param str String to match.
 * @return Generated token if matched, nullptr otherwise.
 */
bool Blare::State::match(std::string str, std::shared_ptr<Token>& dest) {
	for (auto i : rules) {
		auto result = i->match(str,dest);
		if (result)
			return result;
	}
	return false;
}

/**
 * @brief Add a lexical rule.
 * @param rule Lexical rule to register.
 */
void Blare::State::addRule(std::shared_ptr<LexRule> rule) {
	rules.push_back(rule);
}

/**
 * @brief Constructor of exception class `lexical error'
 * @param line Line where the error at.
 */
Blare::LexicalError::LexicalError(size_t line) : std::runtime_error("Lexical error") {
	this->line = line;
}

Blare::LexicalError::~LexicalError() {}

/**
 * @brief Get the line where the error at.
 * @return Line where the error at.
 */
size_t Blare::LexicalError::getLine() {
	return line;
}
