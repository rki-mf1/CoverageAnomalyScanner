#include "CoverageAgent.h"
//#include "Kmeans.h"       // legacy
#include "BreakpointFinder.h"
#include "VCFwriter.h"
#include "parser.h"
#include <cassert>

#ifdef PRINT
#include <fstream>
#endif

using namespace std;



int main(int argc, char const *argv[]){

    argparse::ArgumentParser parser("cas", VERSION);
    create_parser(parser, argc, argv);

    const char *bam_file  = parser.get<string>("--bam").c_str();
    int bam_chromosome_id = parser.get<int>("--chr");
    /*  HTSlib works with 0-based half-open intervals. Alignment coordinates in SAM format are 1-based.
    *  Hence, in order to match with the intervals of samtools, only the start position is converted here.
    *  Reference: https://github.com/samtools/htslib/blob/36312fb0a06bd59188fd39a860055fbb4dd0dc63/htslib/hts.h#L1190 
    */
    int bam_start         = parser.get<int>("--start") - 1;
    int bam_end           = parser.get<int>("--end");


    CoverageAgent ca(bam_file, bam_chromosome_id, bam_start , bam_end);

    const uint l_coverages = bam_end - bam_start;
    vector<uint32_t> coverages(l_coverages, 0);
    ca.getGenomeCoverage(coverages);

    vector<float> xFoldChange(l_coverages, 0);
    bool log2wasapplied = ca.getConsecutivePairwiseFoldchange(xFoldChange, coverages);

#ifdef PRINT
    
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

    ocsv.close();
#endif

    BreakpointFinder bf;
    bf.setThreshold(xFoldChange);

    vector<unsigned> startPos;
    vector<unsigned>   endPos;
    bf.findBreakpoints(startPos, endPos, xFoldChange);

    // TODO: next block is DEBUG only
    assert(startPos.size() == endPos.size());
    for (size_t idx = 0; idx < startPos.size(); ++idx){
        cout << "[" << startPos[idx] <<", " << endPos[idx] << "]" << endl;
    }

    VCFwriter vcfwriter("out.vcf");
    vcfwriter.write(bam_file, bam_chromosome_id, bam_start, startPos, endPos);

    return 0;
}