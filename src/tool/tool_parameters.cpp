/*******************************************************************************
 * Copyright (C) 2020 Théo Cavinato, University of Lausanne
 * Copyright (C) 2020 Olivier Delaneau, University of Lausanne
 * Copyright (C) 2020 Simone Rubinacci, University of Lausanne
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include <tool/tool_header.h>

void tool::declare_options() {
	bpo::options_description opt_base ("Basic options");
	opt_base.add_options()
			("help", "Produce help message")
			("seed", bpo::value<int>()->default_value(15052011), "Seed of the random number generator")
			("threads", bpo::value< int>()->default_value(1), "Number of threads")
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
	vrb.bullet("Author        : Théo Cavinato, Simone Rubinacci, Olivier Delaneau");
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
	vrb.bullet("Number of generations: " + to_string(options["gen"].as < int > ()));
	vrb.bullet("Output TXT : [" + options["output"].as < string > () + "]");
	if (options.count("log")) vrb.bullet("Output LOG : [" + options["log"].as < string > () + "]");
}

void tool::verbose_options() {
	vrb.title("Parameters:");
	vrb.bullet("Seed       : " + stb.str(options["seed"].as < int > ()));
}
