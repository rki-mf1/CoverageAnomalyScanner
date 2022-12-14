/**
 * @file     src/VCFwriter.h
 * @brief    This file contains the Class to write a VCF file using htslib
 * 
*/
#ifndef VCFWRITER_
#define VCFWRITER_

#include <htslib/vcf.h>
#include <htslib/sam.h>
#include <iostream>
#include <string>
#include <vector>
#include "parser.h"


class VCFwriter{

private:

    const char* vcf_file_;


public:
    VCFwriter(const char* ofile):
        vcf_file_(ofile)
        {}

    /**
     * @fn      write
     * @brief   Function to write the output VCF file.
     * @param   f_bam is the name of the input BAM file
     * @param   tid is the 0-based index of the chromosome name as in the BAM file
     * @param   window_start_pos is the start position of the observed genomic window
     * @param   final_threshold is the threshold that was ultimately used bythe program
     * @param   startPos vector to store starting SV breakpoints
     * @param   endPos vector to store ending SV break breakpoints
     * @return  0 for success, 1 for failure
    */
    int write(const char* f_bam,
              const int tid,
              const int window_start_pos,
              const float final_threshold,
              const std::vector<unsigned> &startPos,
              const std::vector<unsigned> &endPos) const;


    /**
     * @fn      write_range
     * @brief   Wrapper for write() if --range was used as input.
    */
    int write_range(argparse::ArgumentParser &parser,
                    range_t &range,
                    const float final_threshold,
                    const std::vector<unsigned> &startPos,
                    const std::vector<unsigned> &endPos) const;

};






#endif /* VCFWRITER_ */