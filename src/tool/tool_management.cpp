#include <tool/tool_header.h>

tool::tool() {
}

tool::~tool() {
}

void tool::run(vector < string > & args) {
	declare_options();
	parse_command_line(args);
	check_options();
	verbose_files();
	verbose_options();
	read_files_and_initialise();
	runMainTask();
	write_files_and_finalise();
}
