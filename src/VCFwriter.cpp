#include "VCFwriter.h"



int VCFwriter::init_hdr(const char* f_bam, const int tid) const{


    bcf_hdr_t *vcf_hdr = bcf_hdr_init("w");
    if (!vcf_hdr){
        std::cerr << "htslib/vcf.h:bcf_hdr_init() failed." << std::endl;
        return 1;
    }

    // add version line into vcf header
    bcf_hdr_set_version(vcf_hdr, "VCFv4.2");

    // TODO: add timestamp
    // TODO: add program name
    // TODO: try getting the reference from BAM's @PG:CL field if available (e.g. as with bwa)

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
    int res = bcf_hdr_write(vcf_buffer, vcf_hdr);

    if (res!=0){
        std::cerr << "htslib/vcf.h:bcf_hdr_write() failed." << std::endl;
        return 1;
    }

    // clean up
    hts_close(vcf_buffer);
    bcf_hdr_destroy(vcf_hdr);

    return 0;
}