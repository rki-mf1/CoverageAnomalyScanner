/**
 * @file     src/Kmeans.h
 * @brief    This file contains the Class to perform the k-means optimization
 * 
*/
#ifndef KMEANS_
#define KMEANS_

#include <dlib/clustering.h>        // kmeans clustering
#include <stdint.h>                 // fixed range integer
#include <vector>
#include <algorithm>                // std::min_element(), max_element()
#include <utility>                  // std::pair


class Kmeans{

    typedef dlib::vector<uint,2> kmeans_data_t;

    std::vector<kmeans_data_t> datapoints_;
    std::vector<kmeans_data_t> centers_;


public:
    /**
     * @fn      [Constructor]
     * @brief   Function to initiate a Kmeans instance
     * @return  
    */
    Kmeans() = default;

    /**
     * @fn      k2mm_means
     * @brief   Function to compute two centers via k-means optimization.
     * @details Function computes two center points via k-means optimization. The data must be in 1D, where dlib::vector::y() = 1 for all data points.
     *          The seed for the two centers are at the minimum and maximum of the input data.
     * @param   datapoints is a vector of (artificial) 1D numerical data to find centers for
     * @return  a std::pair (c1, c2) where c1 and c2 are the center points of the 1D data clusters
    */
    std::pair<uint, uint> k2mm_means(const std::vector<uint32_t> &datapoints);

    /**
     * @fn      clear
     * @brief   Function clears internal data structures.
     * @return  void
    */
    void clear();
};













#endif /*KMEANS_*/