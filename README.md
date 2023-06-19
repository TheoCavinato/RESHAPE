# <p align="center"> <img height="150" src="logo.png"/> <br>RESHAPE </p>

## Paper
For more information please read this [manuscript](https://www.biorxiv.org/content/10.1101/2023.04.07.535812v1.full.pdf+html) 

## Versions

Current release: v1.0.0. Release date: ?

## License
RESHAPE is available under a MIT license. For more information please see the LICENSE.

## RESHAPE
RESHAPE removes sample-level genetic information from a reference panel to create a synthetic reference panel. By providing it with a genetic map and the VCF/BCF of a reference panel, RESHAPE outputs a VCF/BCF of the same size where each haplotypes corresponds to a mosaic of the original haplotypes of the reference panel.

It takes as input:
* --vcf: reference panel in the VCF or BCF file format
* --region: chromosome to shuffle
* --output: output path for the synthetic reference panel (should be in VCF or BCF file format)
* --map: path to the recombination map
* --seed: the seed for the rng
* --gen: the number of generations (K in the paper) to simulate recombination events

And outputs a synthetic reference panel of the same size as the original reference panel in the VCF or BCF file format.

## Quickstart
./bin/haploshuffling --vcf $INPUT_FILE --region $CHR --output $OUTPUT --map $MAP --seed 13524 --gen 100

