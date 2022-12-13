/*******************************************************************************
 * Copyright (C) 2020 Théo Cavinato, University of Lausanne
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

#include <io/genotype_reader_writer.h>

genotype_reader_writer::genotype_reader_writer(){}

genotype_reader_writer::~genotype_reader_writer(){}

void genotype_reader_writer::set_n_samples(int number_of_samples){
	n_samples = number_of_samples;
}

void genotype_reader_writer::shuffling(){
	for(int i = 0; i<n_samples*2; i++){
		haplotypes_positions.push_back(i);
	}
	std::shuffle(haplotypes_positions.begin(), haplotypes_positions.end(), rng.getEngine());
}

double genotype_reader_writer::linear_conversion(double X, double cM_1, double cM_2, int bp_1, int bp_2){
	return (((double)X-(double)bp_1)/((double)bp_2 - (double)bp_1))*( cM_2 - cM_1)+ cM_1;
}

void genotype_reader_writer::encoding(string fvcfin, string fvcfout, vector<int> &gmap_pos_bp, vector<double> &gmap_pos_cM, vector<double> &recombination_sites, string out_rec, string out_haplo){
	//-----------INITIALISE VCF TO READ---------------//
	//Create input file descriptors
	vrb.bullet("Creating file descriptor");

    //open original file "fvcfin"
    htsFile *fp = hts_open(fvcfin.c_str(), "r"); if ( !fp ) vrb.error("Failed to open: " + fvcfin + ".");
	bcf_hdr_t *hdr = bcf_hdr_read(fp); if ( !hdr ) vrb.error("Failed to parse header: " + fvcfin +".");
    //copy header to out_hdr
	bcf_hdr_t * out_hdr = bcf_hdr_dup(hdr);

	//to read vcf
	unsigned int nset = 0;
	int ngt, *gt_arr = NULL, ngt_arr = 0;
	bcf1_t * rec = bcf_init();
	int n_samples = bcf_hdr_nsamples(hdr);

	//-----------INITIALISE VCF TO WRITE---------------//
    //open output file
	string file_format = "w";
	if (fvcfout.size() > 6 && fvcfout.substr(fvcfout.size()-6) == "vcf.gz") file_format = "wz";
	if (fvcfout.size() > 3 && fvcfout.substr(fvcfout.size()-3) == "bcf")  file_format = "wb";

    htsFile * out_fp = hts_open(fvcfout.c_str(),file_format.c_str());
	if ( out_fp == NULL ) vrb.error("Can't write to " + fvcfout + ".");
	bcf_hdr_add_sample(out_hdr, NULL);
    //write header copied before
	if (bcf_hdr_write(out_fp, out_hdr)) vrb.error("Failed to write header to output file");

	//to write VCF
	int * genotypes_out = (int*)malloc(n_samples*2*sizeof(int));

	//-----------IF YOU WANT LOGS OF THE RECOMBINATION SITES---------------//
	ofstream rec_file;
	ofstream haplo_file;
	if (out_rec!="None"){
		rec_file.open(out_rec);
	}
	if (out_haplo!="None"){
		haplo_file.open(out_haplo);
	}

	//-----------READ VCF AND WRITE MIXED HAPLOTYPES TO NEW ONE---------------//

	//define iterators for gmap's positions in bp, in cM and for simulated recombination sites
	int itr_gmap=0, itr_recsite=0;
	int max_gmap=gmap_pos_bp.size()-1, max_recsite=recombination_sites.size();

	int SNPs = 0;

	vrb.bullet("Encoding start...");
	tac.clock();
	while((nset=bcf_read(fp, hdr, rec))==0) {
		bcf_unpack(rec, BCF_UN_STR);

		ngt = bcf_get_genotypes(hdr, rec, &gt_arr, &ngt_arr);

		//step0. Find the number of recombination
		// make the gmap encompass the SNP position
		while(itr_gmap<max_gmap && gmap_pos_bp[itr_gmap]<rec->pos){ 
			itr_gmap++;
		}

		if(itr_gmap && itr_gmap < max_gmap){ //only consider SNPs comprised in recombinatino maps
			double rec_pos_cM = linear_conversion(rec->pos, gmap_pos_cM[itr_gmap-1], gmap_pos_cM[itr_gmap], gmap_pos_bp[itr_gmap-1], gmap_pos_bp[itr_gmap]);

			// count the number of recombination happening
			int nbr_of_recombinations = 0;
			while(itr_recsite<max_recsite && recombination_sites[itr_recsite]<rec_pos_cM){
				itr_recsite++;
				nbr_of_recombinations++;
			}
			if(itr_recsite<max_recsite){
				//step1. Permute haplotypes_positions depending on the number of recombination events
				for(int i = 0; i<nbr_of_recombinations; i++){
				// FAIS GAFFE JE CRAINS QUE UN HAPLOTYPE PEUT RECOMBINER AVEC LUI MEME ET JE SAIS PAS SI C UN PBLM
				int first_element=rng.getInt(n_samples*2), second_element=rng.getInt(n_samples*2);
				int first_value=haplotypes_positions[first_element], second_value = haplotypes_positions[second_element];
				haplotypes_positions[first_element] = second_value;
				haplotypes_positions[second_element] = first_value;

				//step1. bis Write the recombination event if --recvalid
				if (out_rec!="None"){
					rec_file << rec->pos;
					rec_file << "\n";
					}
				}

				
			}

		}

		//step2. Write genotypes in new order
		for(int i = 0; i < n_samples*2; i++) genotypes_out[i] = bcf_gt_phased(bcf_gt_allele(gt_arr[haplotypes_positions[i]]));
		bcf_update_genotypes(out_hdr, rec, genotypes_out, n_samples*2);
		if (bcf_write(out_fp, out_hdr, rec) < 0) vrb.error("Failing to write VCF/record");
		SNPs+=1;

		if (out_haplo!="None"){
			for(int i =  0; i<n_samples*2; i++){ haplo_file << haplotypes_positions[i] << " "; }
			haplo_file << "\n";
		}

		if (SNPs%100000==0){
			vrb.bullet(to_string(SNPs) + " SNPs written in " + stb.str(tac.rel_time()*1.0/1000, 2) + "s");
		}

	}
	//free memory
    bcf_hdr_destroy(out_hdr);
    if ( hts_close(out_fp)!=0 ) vrb.error("Close failed: " + fvcfout+ ".");
    bcf_hdr_destroy(hdr);
    if ( hts_close(fp)!=0 ) vrb.error("Close failed: " + fvcfin + ".");

	if (out_rec!="None"){
		rec_file.close();
	}
	if (out_haplo!="None"){
		haplo_file.close();
	}	

	vrb.bullet("Shuffling done!");
};

void genotype_reader_writer::update_original_pos(vector<int> &orig_pos, vector<int> &haplo_pos){
	for(int i = 0; i<haplo_pos.size(); i++){
		int idx = 0;
		for(auto j : haplo_pos){
			if (i==j) {
				orig_pos[i]  = idx;
				break;
			}
			idx++;
		}
	}
}

void genotype_reader_writer::decoding(string fvcfin, string fvcfout, vector<int> &gmap_pos_bp, vector<double> &gmap_pos_cM, vector<double> &recombination_sites, string out_rec){
	//-----------INITIALISE VCF TO READ---------------//
	//Create input file descriptors
	vrb.bullet("Creating file descriptor");

    //open original file "fvcfin"
    htsFile *fp = hts_open(fvcfin.c_str(), "r"); if ( !fp ) vrb.error("Failed to open: " + fvcfin + ".");
	bcf_hdr_t *hdr = bcf_hdr_read(fp); if ( !hdr ) vrb.error("Failed to parse header: " + fvcfin +".");
    //copy header to out_hdr
	bcf_hdr_t * out_hdr = bcf_hdr_dup(hdr);

	//to read vcf
	unsigned int nset = 0;
	int ngt, *gt_arr = NULL, ngt_arr = 0;
	bcf1_t * rec = bcf_init();
	int n_samples = bcf_hdr_nsamples(hdr);

	//-----------INITIALISE VCF TO WRITE---------------//
    //open output file
	string file_format = "w";
	if (fvcfout.size() > 6 && fvcfout.substr(fvcfout.size()-6) == "vcf.gz") file_format = "wz";
	if (fvcfout.size() > 3 && fvcfout.substr(fvcfout.size()-3) == "bcf")  file_format = "wb";

    htsFile * out_fp = hts_open(fvcfout.c_str(),file_format.c_str());
	if ( out_fp == NULL ) vrb.error("Can't write to " + fvcfout + ".");
	bcf_hdr_add_sample(out_hdr, NULL);
    //write header copied before
	if (bcf_hdr_write(out_fp, out_hdr)) vrb.error("Failed to write header to output file");

	//to write VCF
	int * genotypes_out = (int*)malloc(n_samples*2*sizeof(int));

	//-----------IF YOU WANT LOGS OF THE RECOMBINATION SITES---------------//
	ofstream rec_file;
	if (out_rec!="None"){
		rec_file.open(out_rec);
	}
	

	//-----------READ VCF AND WRITE MIXED HAPLOTYPES TO NEW ONE---------------//
	//find original haplotypes
	//shuffling
	vector<int> original_pos(n_samples*2);
	update_original_pos(original_pos, haplotypes_positions);

	//define iterators for gmap's positions in bp, in cM and for simulated recombination sites
	int itr_gmap=0, itr_recsite=0;
	int max_gmap=gmap_pos_bp.size()-1, max_recsite=recombination_sites.size();

	int SNPs = 0;

	vrb.bullet("Decoding start...");
	tac.clock();
	while((nset=bcf_read(fp, hdr, rec))==0) {
		bcf_unpack(rec, BCF_UN_STR);

		ngt = bcf_get_genotypes(hdr, rec, &gt_arr, &ngt_arr);

		//step0. Find the number of recombination
		// make the gmap encompass the SNP position
		while(itr_gmap<max_gmap && gmap_pos_bp[itr_gmap]<rec->pos){ 
			itr_gmap++;
		}

		if(itr_gmap && itr_gmap < max_gmap){ //only consider SNPs comprised in recombinatino maps
			double rec_pos_cM = linear_conversion(rec->pos, gmap_pos_cM[itr_gmap-1], gmap_pos_cM[itr_gmap], gmap_pos_bp[itr_gmap-1], gmap_pos_bp[itr_gmap]);

			// count the number of recombination happening
			int nbr_of_recombinations = 0;
			while(itr_recsite<max_recsite && recombination_sites[itr_recsite]<rec_pos_cM){
				itr_recsite++;
				nbr_of_recombinations++;
			}
			if(itr_recsite < max_recsite){
				//step1. Permute haplotypes_positions depending on the number of recombination events
				for(int i = 0; i<nbr_of_recombinations; i++){
					// FAIS GAFFE JE CRAINS QUE UN HAPLOTYPE PEUT RECOMBINER AVEC LUI MEME ET JE SAIS PAS SI C UN PBLM
					int first_element=rng.getInt(n_samples*2), second_element=rng.getInt(n_samples*2);
					int first_value=haplotypes_positions[first_element], second_value = haplotypes_positions[second_element];
					haplotypes_positions[first_element] = second_value;
					haplotypes_positions[second_element] = first_value;

					//step1. bis Write the recombination event if --recvalid
					if (out_rec!="None"){
						rec_file << rec->pos;
						rec_file << "\n";
					}

				}

				update_original_pos(original_pos, haplotypes_positions);

			}


		}


		//step2. Write genotypes in new order
		for(int i = 0; i < n_samples*2; i++) genotypes_out[i] = bcf_gt_phased(bcf_gt_allele(gt_arr[original_pos[i]]));
		bcf_update_genotypes(out_hdr, rec, genotypes_out, n_samples*2);
		if (bcf_write(out_fp, out_hdr, rec) < 0) vrb.error("Failing to write VCF/record");
		SNPs+=1;
		if (SNPs%100000==0){
			vrb.bullet(to_string(SNPs) + " SNPs written in " + stb.str(tac.rel_time()*1.0/1000, 2) + "s");
		}

	}
	//free memory
    bcf_hdr_destroy(out_hdr);
    if ( hts_close(out_fp)!=0 ) vrb.error("Close failed: " + fvcfout+ ".");
    bcf_hdr_destroy(hdr);
    if ( hts_close(fp)!=0 ) vrb.error("Close failed: " + fvcfin + ".");

	if (out_rec!="None"){
		rec_file.close();
	}
	vrb.bullet("Shuffling done!");
}