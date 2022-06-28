#include "parser.h"

using namespace Blare;

Blare::Parser::Parser() {
}

Blare::Parser::~Parser() {
}

void Blare::Parser::parse(
	const Util::ArrayList<std::shared_ptr<Token>>& tokens) {
	for (auto i = tokens.begin(); i != tokens.end(); i++) {

	}
}

Blare::SyntaxError::SyntaxError(size_t line) : runtime_error("Syntax error at line " + std::to_string(line)) {
	this->line = line;
}

Blare::SyntaxError::~SyntaxError() {
}

size_t Blare::SyntaxError::getLine() {
	return line;
}
