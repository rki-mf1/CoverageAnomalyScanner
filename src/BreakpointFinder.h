/**
 * @file     src/BreakpointFinder.h
 * @brief    This file contains the Class to determine SV breakpoints
 * 
*/
#ifndef BREAKPOINT_FINDER_
#define BREAKPOINT_FINDER_

#include <algorithm>   // std::transform, std::max
#include <numeric>     // std::accumulate, std::inner_product
#include <cmath>       // std::sqrt
#include <cstdint>     // fixed range integer
#include <vector>
#include <cassert>



class BreakpointFinder{

private:
    // breakpoint threshold
    float threshold_;

public:
    /**
     * @fn      findBreakpoints
     * @brief   Function to get the breakpoints from genomic positions that previously showed a strong change in coverage.
     * @param   startPos vector to store starting SV breakpoints
     * @param   endPos vector to store ending SV break breakpoints
     * @param   inData is a vector storing the change in coverage between consecutive genomic positions
     * @return  0 for success
     *          1 for failure
     *          2 Number of positive and negative coverage foldchanges are not equal
    */
    int findBreakpoints(std::vector<unsigned> &startPos, std::vector<unsigned> &endPos, const std::vector<float> &inData) const;

    /**
     * @fn      setThreshold
     * @brief   Function to set the threshold to report a coverage anomaly.
     * @param   threshold is a user defined constant as outlier threshold
     * @return  void
    */
    void setThreshold(const float threshold);

    /**
     * @fn      setThreshold
     * @brief   Function to set the threshold to report a coverage anomaly.
     * @param   inData is the vector of signals to automatically determine the outlier threshold
     * @return  void
    */
    void setThreshold(const std::vector<float> &inData);

    /**
     * @fn      getThreshold
     * @brief   Function returning the coverage anomaly threshold.
     * @return  float
    */
    float getThreshold() const;


private:
    // tests for less-equal and greater-equal threshold_, respectively
    bool leqt(float val) const{ return (val <= -1*this->threshold_); }
    bool geqt(float val) const{ return (val >=    this->threshold_); }

    /**
     * @fn      sd
     * @brief   Function to determine the standard deviation of a vector of values.
     * @param   inData is a vector of input values.
     * @return  standard deviation of values in inData
    */
    float sd(const std::vector<float> &inData) const;  

    /**
     * @fn      verify_DUP_pattern
     * @brief   Function to verify if the predicted breakpoints exhibit a DUP pattern.
     *          In a DUP pattern every starting increase in coverage requires a closing decrease in coverge.
     * @param   posChanges is a vector of bools per bp position indicating positive fold changes
     * @param   negChanges is a vector of bools per bp position indicating negative fold changes
     * @return  true, if DUP pattern
    */
    inline bool verify_DUP_pattern(const std::vector<bool> &posChanges, const std::vector<bool> &negChanges) const;

    /**
     * @fn      verify_DEL_pattern
     * @brief   Function to verify if the predicted breakpoints exhibit a DEL pattern.
     *          In a DEL pattern every starting decrease in coverage requires a closing increase in coverge.
     * @param   posChanges is a vector of bools per bp position indicating positive fold changes
     * @param   negChanges is a vector of bools per bp position indicating negative fold changes
     * @return  true, if DEL pattern
    */
    inline bool verify_DEL_pattern(const std::vector<bool> &posChanges, const std::vector<bool> &negChanges) const;

};







#endif /* BREAKPOINT_FINDER_ */