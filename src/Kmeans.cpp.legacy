#include "Kmeans.h"



std::pair<uint, uint> Kmeans::k2mm_means(const std::vector<uint32_t> &datapoints){

    for (std::vector<uint32_t>::const_iterator it = datapoints.cbegin(); it != datapoints.cend(); ++it){
        datapoints_.push_back(kmeans_data_t(*it, 1));
    }

    std::vector<uint32_t>::const_iterator emin = std::min_element(datapoints.cbegin(), datapoints.cend());
    std::vector<uint32_t>::const_iterator emax = std::max_element(datapoints.cbegin(), datapoints.cend());
    centers_.push_back(kmeans_data_t(*emin, 1));
    centers_.push_back(kmeans_data_t(*emax, 1));

    dlib::find_clusters_using_kmeans(datapoints_, centers_);

    return std::make_pair(centers_[0].x(), centers_[1].x());
}


void Kmeans::clear(){
    datapoints_.clear();
    centers_.clear();
}