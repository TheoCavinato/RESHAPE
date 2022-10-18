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

#ifndef _GENOTYPE_READER_WRITER_H
#define _GENOTYPE_READER_WRITER_H

#include <utils/otools.h>

class genotype_reader_writer{
public:
    //CONSTRUCTOR/DESCTRUCTOR
    genotype_reader_writer();
    ~genotype_reader_writer();

    //ROUTINES
    void readAndWriteGenotypes(string fvcfin, string fvcfout, vector<int> &gmap_pos_bp, vector<double> &gmap_pos_cM, vector<double> &recombination_sites);

private:
    double linear_conversion(double X, double cM_1, double cM_2, int bp_1, int bp_2);

};

#endif