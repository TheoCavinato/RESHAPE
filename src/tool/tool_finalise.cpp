#include <tool/tool_header.h>

void tool::write_files_and_finalise() {
	vrb.title("Finalization:");
	vrb.bullet("Total running time = " + stb.str(tac.abs_time()) + " seconds");
}
