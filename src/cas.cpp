#include "CoverageAgent.h"      // imports htslib/sam.h
//#include "Kmeans.h"           // legacy
#include "BreakpointFinder.h"
#include "VCFwriter.h"
#include "parser.h"
#include <cassert>
#include "BEDwriter.h"


#ifdef PRINT
#include <fstream>
#endif

using namespace std;



int main(int argc, char const *argv[]){

    argparse::ArgumentParser parser("cas", VERSION);
    create_parser(parser, argc, argv);
    
    const char *bam_file  = parser.get<string>("--bam").c_str();

    int bam_chromosome_id;
    int bam_start;                  /*  HTSlib works with 0-based half-open intervals. Alignment coordinates in SAM format are 1-based. Hence, in order to match with the intervals of samtools, only the start position is converted here. Reference: https://github.com/samtools/htslib/blob/36312fb0a06bd59188fd39a860055fbb4dd0dc63/htslib/hts.h#L1190 */
    int bam_end;
    
    range_t r;
    if (parser.is_used("--range")){

        r = parse_range(parser);
    
        bam_chromosome_id = r.tid_;
        bam_start         = r.beg_;     // htslib adjusts this automatically
        bam_end           = r.end_;
    }
    else{
        bam_chromosome_id = parser.get<int>("--chr");
        bam_start         = parser.get<int>("--start") - 1;
        bam_end           = parser.get<int>("--end");

        /* TODO: The following three line are for consistency only. The program should
           ultimately only use range_t over three separate variables. */
        r.tid_ = bam_chromosome_id;
        r.beg_ = bam_start;
        r.end_ = bam_end;
    }

    CoverageAgent ca(bam_file, bam_chromosome_id, bam_start , bam_end);

    const uint l_coverages = bam_end - bam_start;
    vector<uint32_t> coverages(l_coverages, 0);
    ca.getGenomeCoverage(coverages);

    vector<float> xFoldChange(l_coverages, 0);
    bool log2wasapplied = ca.getConsecutivePairwiseFoldchange(xFoldChange, coverages);

    BreakpointFinder bf;
    if (parser.is_used("--threshold")){
        const float user_threshold = parser.get<float>("--threshold");
        bf.setThreshold(user_threshold);
    }
    else{
        bf.setThreshold(xFoldChange);
    }

    const float final_threshold = bf.getThreshold();
    print_parser(parser, final_threshold);    

#ifdef PRINT            // write CSV
    
    ofstream ocsv;
    ocsv.open("coverage.csv");
    if (! ocsv.is_open() ){
        cerr << "[Error] Could not open file buffer for coverage statistics." << endl;
    }
    
    // print raw coverage
    ocsv << "raw_coverage";
    for (size_t i = 0; i < l_coverages; ++i){
        ocsv << "\t" << coverages[i];
    }
    ocsv << "\n";

    // print pairwise consecutive (log2) x-fold change
    log2wasapplied ? ocsv << "log2fold_change" : ocsv << "fold_change";
    for (size_t i = 0; i < l_coverages; ++i){
        ocsv << "\t" << xFoldChange[i];
    }
    ocsv << "\n";

    // print coverage threshold
    const float thr = bf.getThreshold();
    ocsv << "threshold\t" << thr << "\n";

    ocsv.close();
#endif

    vector<unsigned> startPos;
    vector<unsigned>   endPos;
    bf.findBreakpoints(startPos, endPos, xFoldChange);

    // Report pairs (if possible)
    assert(startPos.size() == endPos.size());
    if (startPos.size() == 0){
        cout << "[Window positions]\t<NO POSITIONS REPORTED>\n";
        return 0;
    }    

    // print positions
    cout << "[Window positions]\t";
    for (size_t idx = 0; idx < startPos.size(); ++idx){
        cout << "[" << startPos[idx] <<", " << endPos[idx] << ")" << endl;
    }

    // write BED file   (records are 0-based half-closed-half-open intervals)
    BEDwriter bedwriter("predictedEvents.bed");
    (bedwriter.write(bam_file, r, startPos, endPos)) ?
        cerr << "[Output] Warning: Couldn't write BED file." << endl :
        cout << "[Output] Wrote BED file." << endl;

    // write VCF file   (records are 1-based closed intervals)
    VCFwriter vcfwriter("predictedEvents.vcf");
    int ret_vcf_failed = 0;
    (parser.is_used("--range")) ?
        ret_vcf_failed = vcfwriter.write_range(parser, r, final_threshold, startPos, endPos) :
        ret_vcf_failed = vcfwriter.write(bam_file, bam_chromosome_id, bam_start, final_threshold, startPos, endPos);
    (ret_vcf_failed) ?
        cerr << "[Output] Warning: Couldn't write VCF file." << endl :
        cout << "[Output] Wrote VCF file." << endl;

    return 0;
}