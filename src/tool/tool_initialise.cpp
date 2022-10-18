#include <tool/tool_header.h>


void tool::read_files_and_initialise() {
	vrb.title("Initialization:");

	//step0: Initialize seed and multi-threading
	rng.setSeed(options["seed"].as < int > ());
	vrb.bullet("Seed	: " + to_string(rng.getSeed()));

	//step1: Read genetic map
	//read the genetic map for the correct chromosome 
	GMAP.readGeneticMapFile(options["map"].as < string > ());

	//step2: Count number of individuals
	HREADER.count_number_of_samples(options["vcf"].as <string> ());

}
