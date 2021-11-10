#include "Lisp.hpp"

#include <stdio.h>

void readFile(const std::string& fileName, std::string& content)
{
	FILE *file = fopen(fileName.c_str(), "r");

	if (file == NULL) {
		return;
	}

	fseek (file , 0, SEEK_END);
	size_t size = ftell(file);
	rewind (file);

	content.resize(size);

	size = fread(&content[0], 1, size, file);

	fclose(file);
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: lisp <filename>\n");
		return 1;
	}

	var args;
	for (unsigned int i = 1; i < argc; i++) {
		args.push(argv[i]);
	}

	std::string fileContent;
	readFile(argv[1], fileContent);

	fileContent = std::string("(") + fileContent + std::string(")");

	var parsedFileContent;
	parsedFileContent.decode(fileContent);

	Lisp lisp;

	lisp.set("args", args);

	lisp.eval(parsedFileContent);

	return 0;
}
