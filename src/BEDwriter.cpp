#include "BEDwriter.h"



int BEDwriter::write(const char* bam,
                     const range_t range_obj,
                     const std::vector<unsigned> &startPos,
                     const std::vector<unsigned> &endPos) const{

    // get chromosome name from tid & header
    htsFile *fp;
    bam_hdr_t *hdr;

    if (!(fp = sam_open(bam, "r"))) {
        perror(bam);
        exit(1);
    }

    if (!(hdr = sam_hdr_read(fp))) {
        fprintf(stderr, "[Parser] Could not read BAM file header\n");
        exit(1);
    }

    if (range_obj.tid_ < 0){
        std::cout << "[Output] Warning: tid chromosome index is less than zero." << std::endl;
        return 1;
    }

    const char* chr = sam_hdr_tid2name(hdr, range_obj.tid_);
    if (chr==NULL){
        std::cout << "[Output] Warning: Couldn't get chromosome name from tid." << std::endl;
        return 1;
    }

    // write BED records
    std::ofstream bedfile(bedfile_name_);
    if (!bedfile.is_open()){
        std::cout << "[Output] Warning: Couldn't open BED file output stream." << std::endl;
        return 1;
    }

    for (unsigned i = 0; i < startPos.size(); ++i){
        bedfile << chr << "\t" << startPos[i]+range_obj.beg_ << "\t" << endPos[i]+range_obj.beg_ << "\n";
    }

    bedfile.close();

    sam_hdr_destroy(hdr);
    sam_close(fp);
    
    return 0;
}

