#include "parse.h"
#include <stdio.h>

int main(int argc, char** argv) {
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	std::shared_ptr<ExampleLexer> lexer = std::make_shared<ExampleLexer>();
	std::shared_ptr<ExampleParser> parser = std::make_shared<ExampleParser>();

	if (argc != 2) {
		// Interactive mode
		while (true) {
			printf(">");

			std::string src;
			while (true)
			{
				int c = getc(stdin);
				if (c == '\n') {
					if (*(std::prev(src.end())) == '\\') {
						src.erase(std::prev(src.end()));
						continue;
					}
					break;
				}
				src += (char)c;
			}

			try {
				auto tokens = lexer->lex(src);
				parser->parse(tokens);
			} catch (Blare::LexicalError e) {
				printf("Error at line %zu: %s\n", e.getLine() + 1, e.what());
			} catch (Blare::SyntaxError e) {
				printf("Error at line %zu: %s\n", e.getToken()->getLine() + 1, e.what());
			}
		}
	}

	FILE* fp;
	if (!(fp = fopen(argv[1], "r"))) {
		puts("Error opening source file");
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buf = new char[fileSize]{ 0 };
	if (!fread(buf, fileSize, 1, fp)) {
		fclose(fp);
		puts("Error reading soruce file");
		return 1;
	}
	fclose(fp);

	std::string src(std::min(strlen(buf), fileSize), '0');
	strncpy(&*(src.begin()), buf, std::min(strlen(buf), fileSize));
	delete[] buf;

	auto tokens = lexer->lex(src);
	try {
		parser->parse(tokens);
	} catch (Blare::SyntaxError e) {
		printf("Error at line %zu: %s\n", e.getToken()->getLine() + 1, e.what());
	}
	return 0;
}
