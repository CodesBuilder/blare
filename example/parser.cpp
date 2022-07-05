#include "parser.h"

ExampleParser::ExampleParser() {
	addRule(
		RULE_TEST,
		Blare::ParseRule::make(
			{
				{ '$', RULE_TESTSUB },
				{ '@', RULE_TESTSUB }
			},
			[](Blare::TokenList tokens) -> void {
				puts("\nTest matched");
			}));
	addRule(
		RULE_TESTSUB,
		Blare::ParseRule::make(
			{
				{ TOKEN_SYMBOL, '(', ')' }
			},
			[](Blare::TokenList tokens) -> void {
				puts("\nTestsub matched");
			}));
}

ExampleParser::~ExampleParser() {
}
