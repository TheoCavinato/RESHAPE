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

#define MODE_MAF	0
#define MODE_MAC	1
#define MODE_AF		2
#define MODE_AC		3

void tool::runMainTask() {
	vrb.title("Compute main TASK");

	int number_of_samples = HREADER.n_samples;
	int number_of_generations = options["gen"].as <int> ();

	//step0: Shuffling of original haplotype order
	GEN.set_n_samples(number_of_samples);
	GEN.shuffling();

	//step1: Simulating recombination sites in cM using a Poisson Process
	double chromosome_size_M = GMAP.pos_cm.back()/100;
	REC_SITES.poisson_process(number_of_samples, number_of_generations, chromosome_size_M);

	//step2: Read VCF and output mixed haplotypes 
	if (options["mode"].as <string>()=="encoding"){
		GEN.encoding(options["vcf"].as <string>(), options["output"].as <string>(),
		GMAP.pos_bp, GMAP.pos_cm, REC_SITES.recombination_sites_cM,
		options["recvalid"].as <string>(), options["haploprint"].as <string>());
	}
	else if(options["mode"].as <string>()=="decoding"){
		GEN.decoding(options["vcf"].as <string>(), options["output"].as <string>(),
		GMAP.pos_bp, GMAP.pos_cm, REC_SITES.recombination_sites_cM,
		options["recvalid"].as <string>());
	}
	else{
		vrb.error("Mode \"" + options["mode"].as <string>() + "\" unknown, please choose between encoding or decoding or leave default");
	}
}
