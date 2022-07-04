#include "parser.h"

ExampleParser::ExampleParser() {
	addRule(
		RULE_TEST,
		Blare::ParseRule::make(
			{ ';', RULE_TESTSUB },
			[](Blare::TokenList tokens) -> void {
				puts("\nTest matced");
			}));
	addRule(
		RULE_TESTSUB,
		Blare::ParseRule::make(
			{ TOKEN_SYMBOL, '(', ')' },
			[](Blare::TokenList tokens) -> void {
				puts("\nTestsub matched");
			}));
}

ExampleParser::~ExampleParser() {
}
