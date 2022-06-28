#ifndef __BLARE_LEXER_H__
#define __BLARE_LEXER_H__

#include "base.h"
#include "util/container.hpp"
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <regex>
#include <string>

namespace Blare {
	class LexicalError : std::runtime_error {
	protected:
		size_t line;

	public:
		explicit LexicalError(size_t line);
		virtual ~LexicalError();
		virtual size_t getLine();
	};

	class Token {
	protected:
		int token;
		size_t line;

	public:
		Token(int token, size_t line);
		virtual ~Token();
		virtual int getToken();
		virtual void setToken(int token);
		virtual size_t getLine();
		virtual void setLine(size_t line);

		static inline std::shared_ptr<Token> make(int token, size_t line) {
			return std::make_shared<Token>(token, line);
		}

		Token& operator=(Token&) = delete;
	};

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
		ValuedToken(int token, size_t line, T value) : Token(token, line) {
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

		static inline std::shared_ptr<ValuedToken<T>> make(int token, size_t line, T data) {
			return std::make_shared<ValuedToken<T>>(token, line, data);
		}
	};

	/**
	 * @brief Lexical rule class, each instance represents a single lexical rule.
	 */
	class LexRule {
	public:
		using ThenProc = std::function<std::shared_ptr<Token>(std::smatch)>;

		LexRule(std::regex rule, ThenProc then);
		virtual ~LexRule();

		virtual std::shared_ptr<Token> match(std::string str);

		LexRule& operator=(LexRule&) = delete;

	protected:
		std::regex rule; // REGEX for matching
		ThenProc then;	 // What to execute if matched
	};

	using StateID = int;
	/**
	 * @brief Lexical analyser state class.
	 */
	class State {
	protected:
		std::map<int, std::shared_ptr<LexRule>> rules;

	public:
		virtual ~State();
		virtual std::shared_ptr<Token> match(std::string str);

		virtual void registerRule(int id, std::shared_ptr<LexRule> rule);
		/**
		 * @brief A wrap for `registerRule` method.
		 * @tparam T Type of parameter `rule`, must be a type which can cast to `std::regex`.
		 * @param id ID for new rule.
		 * @param rule A value that can cast to a `std::regex` object.
		 * @param then What to do if matched.
		*/
		template<typename T>
		inline void registerRule(int id, T rule, LexRule::ThenProc then) {
			registerRule(id, std::make_shared<Blare::LexRule>(std::regex(rule), then));
		}

		virtual void unregisterRule(int id);
		virtual std::weak_ptr<LexRule> getRule(int id);
		virtual bool hasRule(int id);

		State& operator=(State&) = delete;
	};

	/**
	 * @brief The lexical analyser base class, you can create a subclass and register states in the constructor.
	 * @note DO NOT create the base class directly!
	 */
	class Lexer {
	protected:
		static constexpr int STATE_INITIAL = 0; // Initial state, please register it manually in the subclass.

		std::map<StateID, std::shared_ptr<State>> states; // Registered states
		StateID currentState;							  // Current state

		virtual void registerState(StateID id, std::weak_ptr<State> state);
		virtual void unregisterState(StateID id);

	public:
		Lexer();
		virtual ~Lexer();
		virtual Util::ArrayList<std::shared_ptr<Token>> lex(std::string src);

		Lexer& operator=(Lexer&) = delete;
	};
}

#endif