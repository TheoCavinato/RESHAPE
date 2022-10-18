#define _DECLARE_TOOLBOX_HERE
#include <iostream>
#include <tool/tool_header.h>

using namespace std;

int main(int argc, char ** argv) {

	vector < string > args;
	for (int a = 1 ; a < argc ; a ++) args.push_back(string(argv[a]));
	tool().run(args);

	return 0;
}

