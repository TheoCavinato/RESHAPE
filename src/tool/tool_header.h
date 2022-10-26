#ifndef _TOOL_H
#define _TOOL_H

#include <utils/otools.h>

#include <io/gmap_reader.h>
#include <containers/recombination.h>
#include <io/genotype_reader_writer.h>
#include <io/bcf_header_reader.h>

class tool {
public:
	//COMMAND LINE OPTIONS
	bpo::options_description descriptions;
	bpo::variables_map options;

	//INPUT DATA
	gmap_reader GMAP;
	recombination REC_SITES;
	genotype_reader_writer GEN;
	bcf_header_reader HREADER;

	//OUTPUT DATA

	//CONSTRUCTOR
	tool();
	~tool();

	//METHODS
	void runMainTask();
	void run(vector < string > &);

	//PARAMETERS
	void declare_options();
	void parse_command_line(vector < string > &);
	void check_options();
	void verbose_options();
	void verbose_files();

	//
	void read_files_and_initialise();
	void write_files_and_finalise();
};


#endif


