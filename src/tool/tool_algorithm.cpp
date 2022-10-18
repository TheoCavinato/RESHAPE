#include <tool/tool_header.h>

#define MODE_MAF	0
#define MODE_MAC	1
#define MODE_AF		2
#define MODE_AC		3

void tool::runMainTask() {
	vrb.title("Compute main TASK");

	//step0: Simulating recombination sites in cM using a Poisson Process
	int number_of_samples = HREADER.n_samples;
	int number_of_generations = options["gen"].as <int> ();
	double chromosome_size_M = GMAP.pos_cm.back()/100;
	REC_SITES.poisson_process(number_of_samples, number_of_generations, chromosome_size_M);

	//step0 bis: juste pour la validation
	ofstream rec_file;
	rec_file.open("rec_sites.txt");
	for (auto i : REC_SITES.recombination_sites_cM){
		rec_file << i;
		rec_file << '\n';
	}
	rec_file.close();
	
	//step1: Read VCF and output mixed haplotypes 
	GEN.readAndWriteGenotypes(options["vcf"].as <string>(), options["output"].as <string>(), GMAP.pos_bp, GMAP.pos_cm, REC_SITES.recombination_sites_cM);
}
