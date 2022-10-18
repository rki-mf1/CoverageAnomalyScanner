/**
 * @file     src/BreakpointFinder.h
 * @brief    This file contains the Class to determine SV breakpoints
 * 
*/
#ifndef BREAKPOINT_FINDER_
#define BREAKPOINT_FINDER_

#include <algorithm>   // std::transform
#include <numeric>     // std::accumulate, std::inner_product
#include <cmath>       // std::sqrt
#include <cstdint>     // fixed range integer
#include <vector>



class BreakpointFinder{

private:
    // breakpoint threshold
    float threshold_;

    const static bool POSITIVE = true;
    const static bool NEGATIVE = false;

public:
    /**
     * @fn      findBreakpoints
     * @brief   Function to get the breakpoints from genomic positions that previously showed a strong change in coverage.
     * @param   startPos vector to store starting SV breakpoints
     * @param   endPos vector to store ending SV break breakpoints
     * @param   inData is a vector storing the change in coverage between consecutive genomic positions
     * @return  0 for success, 1 for failure
    */
    int findBreakpoints(std::vector<unsigned> &startPos, std::vector<unsigned> &endPos, const std::vector<float> &inData) const;

    /**
     * @fn      setThreshold
     * @brief   Function to determine the threshold to report a SV breakpoint.
     * @param   inData is a vector of input signals
     * @param   coeff is a coefficient to determine the outlier threshold
     * @return  void
    */
    void setThreshold(const std::vector<float> &inData, const unsigned coeff);

private:
    // tests for greater-equal and less-equal threshold_
    bool leqt(float val) const{ return (val <= -1*this->threshold_); }
    bool geqt(float val) const{ return (val >=    this->threshold_); }

    /**
     * @fn      sd
     * @brief   Function to determine the standard deviation of a vector of values.
     * @param   inData is a vector of input values.
     * @return  standard deviation of values in inData
    */
    float sd(const std::vector<float> &inData) const;  

};







#endif /* BREAKPOINT_FINDER_ */