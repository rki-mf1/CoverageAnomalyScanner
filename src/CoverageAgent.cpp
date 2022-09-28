#include "CoverageAgent.h"



int CoverageAgent::getGenomeCoverage(std::vector<uint32_t> &coverages) const{

    // load BAM+BAi and fill htslib data structures
    htsFile *bam_file = hts_open(path_, "r");
    hts_idx_t *bam_file_index = sam_index_load(bam_file, path_);
    
    // iterator for the SAM alignment records
    hts_itr_t *iter = sam_itr_queryi(bam_file_index, chrom_id_, start_, end_);

    // init data structures for the SAM records
    bam1_t* bam_alignment = bam_init1(); /* Structure for one alignment */
    bam1_core_t bam_alignment_core = bam_alignment->core; /*  Structure for core alignment information */

    // CIGAR information
    uint32_t *cigar; /* CIGAR field from the alignment */
    int n_cigar;    /* Number of cigar operations */

    // TODO
    while(sam_itr_next( bam_file, iter, bam_alignment) > 0){
		
        cigar = bam_get_cigar(bam_alignment);
        bam_alignment_core = bam_alignment->core;
        n_cigar = bam_alignment_core.n_cigar;

        if ((bam_alignment_core.flag&BAM_FQCFAIL) != 0){      /* Don't include alignments that failed the mapper's quality criteria. */
            continue;
        }

        // get alignment start position and length on the reference
        hts_pos_t rspan  = bam_cigar2rlen(n_cigar, cigar);   /* Number of reference bases consumed by the alignment record. Could be exchanged for htslib/sam.h:bam_endpos() for simplicity. */
        hts_pos_t rstart = bam_alignment_core.pos;

        // determine sub-interval to count coverage
        uint covstart = (rstart < start_) ? 0 : rstart - start_;
        uint covend = (rstart+rspan > end_) ? end_-start_ : (rstart+rspan) - start_;

        // increment coverage in observed window
        for (size_t it = covstart; it < covend; ++it){
            coverages[it]++;
        }
    }

    // cleanup
    bam_destroy1(bam_alignment); /* free alignment pointer */
    sam_itr_destroy(iter);
    hts_idx_destroy(bam_file_index); /* free BAM/CRAM index pointer */
    int hts_close_error_value = hts_close(bam_file); /* close the BAM/CRAM file */
    if (hts_close_error_value < 0){
        return 1;   /* Something went wrong with the file handle. */
    }

    return 0;
}

void CoverageAgent::getConsecutivePairwiseDifferences(std::vector<uint32_t> &consPairDiff, const std::vector<uint32_t> &coverages) const{

    for (size_t i = 1; i < coverages.size(); ++i){

        const int32_t diff= coverages[i] - coverages[i-1];
        consPairDiff[i] = std::abs(diff);
    }
    
}