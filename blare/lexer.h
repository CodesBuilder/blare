#ifndef __BLARE_LEXER_H__
#define __BLARE_LEXER_H__

#include <deque>
#include <functional>
#include <map>
#include <regex>
#include <string>
#include <memory>

#include "base.h"
#include "util/container.hpp"

namespace Blare {
	using std::shared_ptr;

	class LexicalError : public std::runtime_error {
	protected:
		size_t line;

	public:
		explicit LexicalError(size_t line);
		virtual ~LexicalError();
		virtual size_t getLine();
	};

	class Token;

	static constexpr TokenID TOKEN_INVALID = INT_MIN;
	using TokenList = Util::ArrayList<shared_ptr<Token>>;

	class Token {
	protected:
		TokenID token;
		size_t line;

	public:
		Token(TokenID token, size_t line);
		virtual ~Token();
		virtual TokenID getToken() noexcept;
		virtual void setToken(TokenID token) noexcept;
		virtual size_t getLine() noexcept;
		virtual void setLine(size_t line) noexcept;

		static inline shared_ptr<Token> make(TokenID token, size_t line) {
			return std::make_shared<Token>(token, line);
		}

		Token& operator=(Token&) = delete;
	};

	constexpr bool isLexicalToken(TokenID id) noexcept {
		return id >= 0;
	}

	/**
	 * @brief Template class for tokens with data.
	 * @tparam T Data type of token.
	 */
	template <typename T>
	class ValuedToken : public Token {
	protected:
		T value;

	public:
		/**
		 * @brief Constructor method.
		 * @param token Value of the token.
		 * @param line Line where the token at.
		 * @param value Initial data of the token.
		 */
		ValuedToken(TokenID token, size_t line, T value) : Token(token, line) {
			this->value = value;
		}
		ValuedToken(TokenID token, T value) : Token(token, 0) {
			this->value = value;
		}
		ValuedToken(T value) : Token(TOKEN_INVALID, 0) {
			this->value = value;
		}
		/**
		 * @brief Get data of the token.
		 * @return Data of the token.
		 */
		virtual T getData() {
			return value;
		}
		/**
		 * @brief Set data of the token.
		 * @param value New data.
		 */
		virtual void setData(T value) {
			this->value = value;
		}

		static inline shared_ptr<ValuedToken<T>> make(TokenID token, size_t line, T data) {
			return std::make_shared<ValuedToken<T>>(token, line, data);
		}
		static inline shared_ptr<ValuedToken<T>> make(TokenID token, T data) {
			return std::make_shared<ValuedToken<T>>(token, data);
		}
		static inline shared_ptr<ValuedToken<T>> make(T data) {
			return std::make_shared<ValuedToken<T>>(data);
		}
	};

	/**
	 * @brief Lexical rule class, each instance represents a single lexical rule.
	 */
	class LexRule {
	public:
		/**
		 * @brief
		 * @param result Match result from the lexical analyser.
		 * @param dest Reference of pointer of token to push, its value is nullptr by default.
		 * @return true Append the token to the list.
		 * @return false Do not append the token to the list.
		 */
		using ThenProc = std::function<bool(std::smatch result, shared_ptr<Token>& dest)>;

		LexRule(std::regex rule, ThenProc then);
		virtual ~LexRule();

		virtual bool match(std::string str, shared_ptr<Token>& dest);

		LexRule& operator=(LexRule&) = delete;

	protected:
		std::regex rule;  // REGEX for matching
		ThenProc then;	  // What to execute if matched
	};

	using StateID = int;
	/**
	 * @brief Lexical analyser state class.
	 */
	class State {
	protected:
		std::deque<shared_ptr<LexRule>> rules;

	public:
		virtual ~State();
		virtual bool match(std::string str, shared_ptr<Token>& dest);

		virtual void addRule(shared_ptr<LexRule> rule);
		/**
		 * @brief A wrap for `addRule` method.
		 * @tparam T Type of rule object, must can be cast to `regex` object.
		 * @param id ID for new rule.
		 * @param rule Rule object.
		 * @param then What to do if matched.
		 */
		template <typename T>
		inline void addRule(T rule, LexRule::ThenProc then) {
			addRule(std::make_shared<Blare::LexRule>((std::regex)rule, then));
		}
		/**
		 * @brief Add a simple rule to the state.
		 * @tparam T Type of rule object, must can be cast to `regex` object.
		 * @param rule Rule object.
		 * @param token Token to give on matched.
		 * @note Set the token as `INVALID_TOKEN` to discard current token and skip matched area.
		 */
		template <typename T>
		inline void addRule(T rule, TokenID token) {
			addRule(
				std::make_shared<Blare::LexRule>(
					std::regex(rule),
					[token](std::smatch result, shared_ptr<Token>& dest) -> bool {
						dest = std::make_shared<Token>(token, 0);
						return true;
					}));
		}

		State& operator=(State&) = delete;
	};

	/**
	 * @brief The lexical analyser base class, you can create a subclass and register states in the constructor.
	 * @note DO NOT create the base class directly!
	 */
	class Lexer {
	protected:
		static constexpr StateID STATE_INITIAL = 0;	 // Initial state, please register it manually in the subclass.

		std::map<StateID, shared_ptr<State>> states;  // Registered states
		StateID currentState;							   // Current state

		virtual void registerState(StateID id, std::weak_ptr<State> state);
		virtual void unregisterState(StateID id);

	public:
		Lexer();
		virtual ~Lexer();
		virtual TokenList lex(std::string src);

		Lexer& operator=(Lexer&) = delete;
	};
}

#endif
