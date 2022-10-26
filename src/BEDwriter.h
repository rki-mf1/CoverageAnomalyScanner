/**
 * @file     src/BEDwriter.h
 * @brief    This file contains the Class to write a bed file
 * 
*/
#ifndef BCFWRITER_
#define BCFWRITER_

#include <htslib/sam.h>
#include "parser.h"
#include <fstream>
#include <string>
#include <vector>


class BEDwriter{

private:

    const char* bedfile_name_;


public:
    BEDwriter(const char* s):
        bedfile_name_(s)
        {}

    /**
     * @fn      write
     * @brief   Function to write the output BED file.
     * @param   f_bam is the name of the input BAM file
     * @param   range_obj is the data class that contains info about the observed window (chr,beg,end)
     * @param   startPos vector to store starting breakpoints in anomaly pairs
     * @param   endPos vector to store ending breakpoints in anomaly pairs
     * @return  0 for success, 1 for failure
    */
    int write(const char* bam,
              const range_t range_obj,
              const std::vector<unsigned> &startPos,
              const std::vector<unsigned> &endPos) const;

};





#endif /* BCFWRITER_ */