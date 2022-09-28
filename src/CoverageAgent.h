/**
 * @file     src/CoverageAgent.h
 * @brief    This file contains the Class to get and manage the coverage information.
 * @details  The CoverageAgent is a class that extracts the coverage information of a given genomic interval
 *           and maintains its associated data like the consecutive pairwise differences in coverage.
*/
#ifndef COVERAGEAGENT_
#define COVERAGEAGENT_

#include <htslib/sam.h>
#include <stdint.h>     // fixed range integer
#include <iostream>     // DEBUG ONLY SO FAR
#include <vector>
#include <stdlib.h>     // std::abs()



class CoverageAgent{

private:
    // input BAM file
    const char *path_;

    // variables to set the iterator range for the genomic coordinated in the BAM file
    int chrom_id_; /* ID of the chromosome as defined in the BAM/CRAM header. First chromosome ID is 0.*/
    int start_; /* starting coordinate: 0-based */
    int end_; /* ending coordinate: 0-based */


public:
    /**
     * @fn      [Constructor]
     * @brief   Function to initiate the CoverageAgent
     * @return  
    */
    CoverageAgent(const char *path, int chrom_id, int start, int end): 
        path_(path),
        chrom_id_(chrom_id),
        start_(start),
        end_(end)                        
        {}

    /**
     * @fn      getGenomeCoverage
     * @brief   Function to compute the read coverage over a given genomic interval from a BAM file.
     * @param   coverages is a STL vector containing the coverage per base position
     * @return  0 for success; 1 for internal error
    */
    int getGenomeCoverage(std::vector<uint32_t> &coverages) const;

    /**
     * @fn      getConsecutivePairwiseDifferences
     * @brief   Function to compute the difference in coverage between consecutive genomic positions.
     * @param   coverages is a STL vector containing the coverage per base position
     * @param   consPairDiff is a STL vector containing the pairwise differences of consecutive positions in coverages[].
     *          Element consPairDiff[i] is the absolute difference between coverages[i-1] and coverage[i].
     * @return  
    */
    void getConsecutivePairwiseDifferences(std::vector<uint32_t> &consPairDiff, const std::vector<uint32_t> &coverages) const;

};













#endif /*COVERAGEAGENT_*/