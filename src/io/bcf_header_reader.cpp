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

#include <io/bcf_header_reader.h>

bcf_header_reader::bcf_header_reader(){}

bcf_header_reader::~bcf_header_reader(){}

void bcf_header_reader::count_number_of_samples(string fvcfin){
    //open original file "fvcfin"
    htsFile *fp = hts_open(fvcfin.c_str(), "r"); if ( !fp ) vrb.error("Failed to open: " + fvcfin + ".");
	bcf_hdr_t *hdr = bcf_hdr_read(fp); if ( !hdr ) vrb.error("Failed to parse header: " + fvcfin +".");

	//count number of samples
	n_samples = bcf_hdr_nsamples(hdr);

	//free memory
    bcf_hdr_destroy(hdr);
    if ( hts_close(fp)!=0 ) vrb.error("Close failed: " + fvcfin + ".");

	vrb.bullet("Input vcf contains " + stb.str(n_samples) + " samples");
};