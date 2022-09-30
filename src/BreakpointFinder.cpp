#include "BreakpointFinder.h"


void BreakpointFinder::findCoverageThreshold(const std::pair<uint, uint> &clusterCenters){

    // right now this is just a split in 1D distance between the clusters
    coverage_threshold_ = std::floor( (clusterCenters.first + clusterCenters.second) / 2 );

}


int BreakpointFinder::findBreakpoints(const std::vector<uint32_t> &consPairDiff){

    std::vector<uint32_t> breakpoint_indexes;
    uint nb_breakpoints = 0;
    
    for (size_t it = 0; it < consPairDiff.size(); ++it){
        
        if (geq(consPairDiff[it])) {
            breakpoint_indexes.push_back(it);
            ++nb_breakpoints;
        }
    }
    
    if (nb_breakpoints != 2){
        /* TODO: implement a rescue/warning/report mechanism here. */
        return 1;
    }

    this->sv_start_ = breakpoint_indexes[0];
    this->sv_end_ = breakpoint_indexes[1];

    return 0;
}