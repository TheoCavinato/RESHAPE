#include <tool/tool_header.h>

void tool::declare_options() {
	bpo::options_description opt_base ("Basic options");
	opt_base.add_options()
			("help", "Produce help message")
			("seed", bpo::value<int>()->default_value(15052011), "Seed of the random number generator")
			("gen", bpo::value<int>(), "Number of generations");

	bpo::options_description opt_input ("Input files");
	opt_input.add_options()
			("vcf,V", bpo::value< string >(), "Genotypes in VCF/BCF format")
			("region,R", bpo::value< string >(), "Target region")
			("map,M", bpo::value< string >(), "Genetic map");

	bpo::options_description opt_output ("Output files");
	opt_output.add_options()
			("output", bpo::value< string >(), "Output file")
			("recvalid", bpo::value< string >()->default_value("None"), "Output recombination sites")
			("haploprint", bpo::value< string >()->default_value("None"), "Print haplotypes (used for validations)")
			("mode", bpo::value< string >()->default_value("encoding"), "Mode of the algorithm: encoding/decoding")
			("log", bpo::value< string >(), "Log file");

	descriptions.add(opt_base).add(opt_input).add(opt_output);
}

void tool::parse_command_line(vector < string > & args) {
	try {
		bpo::store(bpo::command_line_parser(args).options(descriptions).run(), options);
		bpo::notify(options);
	} catch ( const boost::program_options::error& e ) { cerr << "Error parsing command line arguments: " << string(e.what()) << endl; exit(0); }

	if (options.count("help")) { cout << descriptions << endl; exit(0); }

	if (options.count("log") && !vrb.open_log(options["log"].as < string > ()))
		vrb.error("Impossible to create log file [" + options["log"].as < string > () +"]");

	vrb.title("SCANNER");
	vrb.bullet("Author        : Théo Cavinato");
	vrb.bullet("Contact       : theo.cavinato@unil.ch");
	vrb.bullet("Version       : 1.0");
	vrb.bullet("Run date      : " + tac.date());
}

void tool::check_options() {
	if (!options.count("vcf"))
		vrb.error("You must specify a VCF as input using --vcf");

	if (!options.count("map"))
		vrb.error("You must specify a map as input using --map");

	if (!options.count("output"))
		vrb.error("You must specify an output file using --output");

	if (!options.count("gen"))
		vrb.error("You must specify a number of generations using --gen");
}

void tool::verbose_files() {
	vrb.title("Files:");
	vrb.bullet("Input VCF  : [" + options["vcf"].as < string > () + "]");
	vrb.bullet("Number of generations: " + options["gen"].as < int > ());
	vrb.bullet("Output TXT : [" + options["output"].as < string > () + "]");
	if (options.count("log")) vrb.bullet("Output LOG : [" + options["log"].as < string > () + "]");
}

void tool::verbose_options() {
	vrb.title("Parameters:");
	vrb.bullet("Seed       : " + stb.str(options["seed"].as < int > ()));
}
