#include "lexer.h"

using namespace Blare;

/**
 * @brief Constructor of class `Token'.
 * @param token Value of current token.
 * @param line Line where the token at.
 */
Blare::Token::Token(int token, size_t line) {
	this->token = token;
	this->line = line;
}
Blare::Token::~Token() {}

/**
 * @brief Get value of the token.
 * @return Value of the token.
 */
int Blare::Token::getToken() {
	return token;
}

void Blare::Token::setToken(int token) {
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
 * @brief Lexical analyse a string with the lexical analyser.
 * @param src String to analyse.
 * @exception LexicalError Throws if any lexical error has occurred.
 * @return Generated token.
 */
Util::ArrayList<std::shared_ptr<Token>> Blare::Lexer::lex(std::string src) {
	Util::ArrayList<std::shared_ptr<Token>> tokens;
	for (size_t i = 0, line = 0; i < src.size();) {
		size_t end;
		// Try to match contents as more as possible.
		for (end = src.size(); end >= i; end--) {
			auto matchStr = src.substr(i, end - i);
			auto token = states[currentState]->match(matchStr);
			if (token) {
				if (token->getToken() >= TOKENID_VALID_MIN && token->getToken() <= TOKENID_VALID_MAX)
					tokens.push_back(token);
				line += std::count(matchStr.begin(), matchStr.end(), '\n'); // Increase line automatically.
				goto matched;
			}
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
std::shared_ptr<Token> Blare::LexRule::match(std::string str) {
	std::smatch result;
	if (!std::regex_match(str, result, rule))
		return nullptr;
	return then(result);
}

Blare::State::~State() {}

/**
 * @brief Match a string for each rules in the state till anyone was matched or matched nothing.
 * @param str String to match.
 * @return Generated token if matched, nullptr otherwise.
 */
std::shared_ptr<Token> Blare::State::match(std::string str) {
	for (auto i : rules) {
		auto result = i.second->match(str);
		if (result)
			return result;
	}
	return nullptr;
}

/**
 * @brief Register a lexical rule with specified ID.
 * @param id ID for new rule.
 * @param rule Lexical rule to register.
 */
void Blare::State::registerRule(int id, std::shared_ptr<LexRule> rule) {
	rules[id] = rule;
}

/**
 * @brief Get the lexical rule associated with specified ID.
 * @param id Associated ID.
 * @return Associated lexical rule.
 */
std::weak_ptr<LexRule> Blare::State::getRule(int id) {
	return std::weak_ptr<LexRule>(rules[id]);
}

/**
 * @brief Unregister the rule associated with specified ID.
 * @param id Associated ID.
 */
void Blare::State::unregisterRule(int id) {
	rules.erase(id);
}

/**
 * @brief Check if the ID has been registered.
 * @param id ID to check.
 * @return Whether the ID has been registered.
 */
bool Blare::State::hasRule(int id) {
	return rules.count(id) != 0;
}

/**
 * @brief Constructor of exception class `lexical error'
 * @param line Line where the error at.
 */
Blare::LexicalError::LexicalError(size_t line) : std::runtime_error("Syntax error at line " + std::to_string(line)) {
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
