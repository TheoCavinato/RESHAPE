#------------------------------------------------------------------------------# 
# 1. Encode data
#------------------------------------------------------------------------------# 
echo "ENCODING..."

CHR=22

INPUT_FILE=example_ref_panel.bcf
MAP=maps/genetic_maps.b38/chr$CHR.b38.gmap.gz
SEED=13245
GEN=128
SHUFFLED=shuffled_ref_panel.bcf
HAPLO=haplo_order.txt
./bin/haploshuffling --vcf $INPUT_FILE --region $CHR --output $SHUFFLED --map $MAP --seed $SEED --gen $GEN --haplo $HAPLO
tabix -p bcf $SHUFFLED

#------------------------------------------------------------------------------# 
# 2. Do we have the same number of SNPs?
#------------------------------------------------------------------------------# 
echo "---------------------------------------------------"
echo "Stats from original and shuffled reference panels:"
bcftools index $INPUT_FILE --stats
bcftools index $SHUFFLED --stats

#------------------------------------------------------------------------------# 
# 3. Do we maintain allele frequencies?
#------------------------------------------------------------------------------# 
ORIFREQ=original_freq.txt
SHUFFFREQ=shuffled_freq.txt
bcftools query -f '[%GT]\n' $INPUT_FILE | awk '{print gsub(/0/,"") "\t" gsub(/1/,"")}' > $ORIFREQ
bcftools query -f '[%GT]\n' $SHUFFLED | awk '{print gsub(/0/,"") "\t" gsub(/1/,"")}' > $SHUFFFREQ

echo "---------------------------------------------------"
echo "Allele frequencies differences:"
diff $ORIFREQ $SHUFFFREQ # should return nothing

rm $ORIFREQ $SHUFFFREQ 

#------------------------------------------------------------------------------# 
# 4. Is the decoding correct?
#------------------------------------------------------------------------------# 
echo "---------------------------------------------------"
echo "DECODING.."
UNSHUFF=unshuffled_ref_panel.bcf
./bin/haploshuffling --vcf $SHUFFLED --region $CHR --output $UNSHUFF --map $MAP --seed $SEED --gen $GEN --mode decoding
tabix -p bcf $UNSHUFF

DECOMPORI=decompressed_original_ref_panel.vcf
DECOMPUNSHUFF=decompressed_unshuffled_ref_panel.vcf
bcftools view $INPUT_FILE > $DECOMPORI
bcftools view $UNSHUFF > $DECOMPUNSHUFF

echo "Differences between original bcf and decoded bcf:"
diff $DECOMPUNSHUFF $DECOMPORI #should just return the dates of "bcftools view"

rm $DECOMPORI $DECOMPUNSHUFF

# remove generated files
rm $SHUFFLED* $UNSHUFF* $HAPLO
