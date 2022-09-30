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


class VCFwriter{

private:
    const char* vcf_file_;


public:
    VCFwriter(const char* ofile):
        vcf_file_(ofile)
        {}


    /**
     * @fn      init_hdr
     * @brief   Function to write the header for the output VCF file.
     * @param   f_bam is the name of the input BAM file
     * @param   tid is the 0-based index of the chromosome name as in the BAM file
     * @return  0 for success, 1 for failure
    */
    int init_hdr(const char* f_bam, const int tid) const;


};



#endif /* VCFWRITER_ */