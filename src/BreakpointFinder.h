/**
 * @file     src/BreakpointFinder.h
 * @brief    This file contains the Class to detect SV breakpoints and write reports in VCF
 * 
*/
#ifndef VARIANT_REPORT_
#define VARIANT_REPORT_

#include <utility>      // std::pair
#include <math.h>       // std::floor()
#include <stdint.h>     // fixed range integer
#include <vector>
#include <algorithm>    // std::find_if


class BreakpointFinder{

private:
    // breakpoint coverage threshold
    unsigned coverage_threshold_;

    // SV breakpoint
    unsigned sv_start_;
    unsigned sv_end_;

public:
    /**
     * @fn      findCoverageThreshold
     * @brief   Function to determine the coverage threshold to report a SV breakpoint.
     * @param   clusterCenters is a tuple of 1D cluster centers
     * @return  void
    */
    void findCoverageThreshold(const std::pair<uint, uint> &clusterCenters);

    /**
     * @fn      findBreakpoints
     * @brief   Function to get the breakpoints from genomic positions that previously showed a strong change in coverage.
     * @param   consPairDiff is a vector storing the change in coverage between consecutive genomic positions
     * @return  0 for success, 1 for failure
    */
    int findBreakpoints(const std::vector<uint32_t> &consPairDiff);

    unsigned getStartBreakpoint() const {return this->sv_start_; }
    unsigned getEndBreakpoint() const {return this->sv_end_; }

private:
    bool geq(uint32_t val) const{ return (val >= this->coverage_threshold_); }

};







#endif /* VARIANT_REPORT_ */