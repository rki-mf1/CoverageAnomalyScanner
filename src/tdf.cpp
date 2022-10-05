#include "CoverageAgent.h"
//#include "Kmeans.h"       // legacy
#include "BreakpointFinder.h"
#include "VCFwriter.h"

#ifdef PRINT
#include <fstream>
#endif

using namespace std;



int main(int argc, char const *argv[]){

    // dummy usage to argc to suppress Wunused warning for now
    (void)argc;

    const char *bam_file = argv[1];
    int bam_chromosome_id = stoi(argv[2]);
    int bam_start = stoi(argv[3]) - 1;  /*  HTSlib works with 0-based half-open intervals. Alignment coordinates in SAM format are 1-based.
                                         *  Hence, in order to match with the intervals of samtools, only the start position is converted here.
                                         *  Reference: https://github.com/samtools/htslib/blob/36312fb0a06bd59188fd39a860055fbb4dd0dc63/htslib/hts.h#L1190 
                                        */
    int bam_end = stoi(argv[4]);

    CoverageAgent ca(bam_file, bam_chromosome_id, bam_start , bam_end);

    const uint l_coverages = bam_end - bam_start;
    vector<uint32_t> coverages(l_coverages, 0);
    ca.getGenomeCoverage(coverages);

    //vector<uint32_t> consPairDiffs(l_coverages, 0);
    //ca.getConsecutivePairwiseDifferences(consPairDiffs, coverages);

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

    //Kmeans km;
    //std::pair<uint, uint> centers;
    
    //centers = km.k2mm_means(consPairDiffs);
    //km.clear();

    //BreakpointFinder bf;
    //bf.findCoverageThreshold(centers);
    //bf.findBreakpoints(consPairDiffs);

    // TODO: nextline is DEBUG only
    //cout << bf.getStartBreakpoint() <<", " << bf.getEndBreakpoint() << endl;

    //VCFwriter vcfwriter("out.vcf");
    //vcfwriter.init_hdr(bam_file, bam_chromosome_id);

    return 0;
}