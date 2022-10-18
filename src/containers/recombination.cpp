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

#include <containers/recombination.h>
#include <random>
#include <algorithm>

recombination::recombination() {}

double recombination::inverse_poisson_CDF(int n_samples, int n_generations){
    return (-log(1-rng.getDouble())/(n_samples*n_generations));
}

void recombination::poisson_process(int n_samples, int n_generations, double chr_size_M){
    tac.clock();
    double first_rec_site_M = inverse_poisson_CDF(n_samples, n_generations);
    // check that the first recombination site is inferior to chromosome size in cM
    if (first_rec_site_M < chr_size_M) recombination_sites_cM.push_back(first_rec_site_M*100);
    else return;

    // simulate recombination_sites
    double last_rec_site_cM = recombination_sites_cM[0];
    while(last_rec_site_cM < chr_size_M*100){
        double interval_M = inverse_poisson_CDF(n_samples, n_generations);
        double new_rec_site_M = interval_M*100 + last_rec_site_cM;
        recombination_sites_cM.push_back(new_rec_site_M);
        last_rec_site_cM = new_rec_site_M;
    }
    recombination_sites_cM.pop_back(); // remove last element which is > chr_size_M

    vrb.bullet("Simulate recombination sites (" + stb.str(tac.rel_time()*1.0/1000, 2) + "s)");
}

recombination::~recombination() {}