#include "VCFwriter.h"



int VCFwriter::write(const char* f_bam, const int tid, const int window_start_pos, const float final_threshold, const std::vector<unsigned> &startPos, const std::vector<unsigned> &endPos) const{

    /* ----------------------------- HEADER SECTION --------------------------------- */

    bcf_hdr_t *vcf_hdr = bcf_hdr_init("w");
    if (!vcf_hdr){
        std::cerr << "htslib/vcf.h:bcf_hdr_init() failed." << std::endl;
        return 1;
    }

    // add version line into vcf header
    bcf_hdr_set_version(vcf_hdr, "VCFv4.2");

    // TODO: add timestamp
    // TODO: add program name
    // TODO: try getting the reference from BAM's @PG:CL field if available (e.g. as with bwa). vcf_validator would really like to see this (warning!). Might add dummy here too.

    // add tool name plus version
    std::string source_line;
    source_line.append("##source=CoverageAnomalyScanner-");
    source_line.append(VERSION);
    const char* c_source_line = source_line.c_str();
    bcf_hdr_append(vcf_hdr, c_source_line);

    // add standard deviation threshold
    std::string threshold_line;
    threshold_line.append("##CAS-threshold=");
    threshold_line.append(std::to_string(final_threshold));
    const char* c_threshold_line = threshold_line.c_str();
    bcf_hdr_append(vcf_hdr, c_threshold_line);

    samFile *bam_buffer = sam_open(f_bam, "r");
    sam_hdr_t *sam_hdr = sam_hdr_read(bam_buffer);

    // add contig line into vcf header
    const char* chrom_name = sam_hdr->target_name[tid];
    const uint32_t chrom_len = sam_hdr->target_len[tid];
    std::string chrom_line;
    chrom_line.append("##contig=<ID=");
    chrom_line.append(chrom_name);
    chrom_line.append(",length=");
    chrom_line.append(std::to_string(chrom_len));
    chrom_line.append(">");
    const char* c_chrom_line = chrom_line.c_str();
    sam_close(bam_buffer);
    sam_hdr_destroy(sam_hdr);
    bcf_hdr_append(vcf_hdr, c_chrom_line);

    // add INFO fields to header
    bcf_hdr_append(vcf_hdr, "##INFO=<ID=SVTYPE,Number=1,Type=String,Description=\"Type of structural variant\">");
    bcf_hdr_append(vcf_hdr, "##INFO=<ID=END,Number=1,Type=Integer,Description=\"End position of the variant described in this record\">");
    bcf_hdr_append(vcf_hdr, "##INFO=<ID=SVLEN,Number=1,Type=Integer,Description=\"Difference in length between REF and ALT allele\">");

    // add ALT fields to header
    bcf_hdr_append(vcf_hdr, "##ALT=<ID=DUP,Description=\"Duplication\">");

    // add headline to header
    bcf_hdr_append(vcf_hdr, "#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO");

    // write header
    vcfFile *vcf_buffer = bcf_open(this->vcf_file_, "w");
    int res1 = bcf_hdr_write(vcf_buffer, vcf_hdr);

    if (res1!=0){
        std::cerr << "htslib/vcf.h:bcf_hdr_write() failed." << std::endl;
        return 1;
    }

    /* ----------------------------- RECORDS SECTION --------------------------------- */
    // TODO: implement loop in case there are more DUPs

    bcf1_t *rec = bcf_init1();
    
    // set ID
    rec->rid = (uint32_t)tid;
    
    // set POS
    rec->pos = window_start_pos + startPos[0]; 
    
    // set REF + ALT                                        // TODO: more precise REF is --reference is given.
    bcf_update_alleles_str(vcf_hdr, rec, "N,<DUP>");        // If no reference is given then the starting base is unknown (hence 'N'). Taking the first base according to the mapped reads might be false in case of a SNV or technical error.

    // set QUAL
    bcf_float_set_missing(rec->qual);

    // set FILTER
    int32_t vcf_rec_filter[1];
    vcf_rec_filter[0] = bcf_hdr_id2int(vcf_hdr, BCF_DT_ID, "PASS");
    bcf_update_filter(vcf_hdr, rec, vcf_rec_filter, 1);

    // set INFO
    bcf_update_info_string(vcf_hdr, rec, "SVTYPE", "DUP");
    uint32_t sv_end = window_start_pos + endPos[0];
    uint32_t sv_len = endPos[0] - startPos[0];
    uint32_t vcf_rec_info_int32[1];
    vcf_rec_info_int32[0] = sv_end;
    bcf_update_info_int32(vcf_hdr, rec, "END", vcf_rec_info_int32, 1);
    vcf_rec_info_int32[0] = sv_len;
    bcf_update_info_int32(vcf_hdr, rec, "SVLEN", vcf_rec_info_int32, 1);
    
    // FORMAT    
    // ?

    if (bcf_write(vcf_buffer, vcf_hdr, rec) != 0){
        std::cerr << "htslib/vcf.h:bcf_write() failed." << std::endl;
        return 1;
    }    

    /* ----------------------------- HTSLIB CLEANUP SECTION --------------------------------- */
    bcf_destroy1(rec);
    bcf_hdr_destroy(vcf_hdr);
    hts_close(vcf_buffer);

    return 0;
}


int VCFwriter::write_range(argparse::ArgumentParser &parser, range_t &range, const float final_threshold, const std::vector<unsigned> &startPos, const std::vector<unsigned> &endPos) const{

    const char *bam_file  = parser.get<std::string>("--bam").c_str();

    const int tid = range.tid_;
    const unsigned start = range.beg_;

    return this->write(bam_file, tid, start, final_threshold, startPos, endPos);
}