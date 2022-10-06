#include "BreakpointFinder.h"


void BreakpointFinder::setThreshold(const std::vector<float> &inData){

    const float sd = this->sd(inData);

    this->threshold_ = 3 * sd;

}


int BreakpointFinder::findBreakpoints(std::vector<unsigned> &startPos, std::vector<unsigned> &endPos, const std::vector<float> &inData) const{

    std::vector<bool> isPositiveFoldChange(inData.size(), false);
    unsigned nb_positive_foldchanges = 0;

    std::vector<unsigned> predicted_breakpoint_indexes;
    unsigned nb_predicted_breakpoint_indexes = 0;
    
    // TODO: for now we only consider non-nested (tandem) duplications being present in the window under investigation
    // TODO: implement a rescue/warning/report mechanism here if the breakpoint pattern does not indicate consecutive DUP / other artifacts

    bool is_geq = false;
    for (size_t idx = 1; idx < inData.size(); ++idx){         // idx=0 == '0' because of the pairwise normalization in the window
        
        is_geq = geqt(inData[idx]);
        if (is_geq || leqt(inData[idx])) {
            
            predicted_breakpoint_indexes.push_back(idx);
            ++nb_predicted_breakpoint_indexes;

            if (is_geq){
                isPositiveFoldChange[idx] = true;           // the trick is here that only positive fold changes become a '1' and negative stay '0', like the dafault.
                ++nb_positive_foldchanges;
            }
        }
    }

    // sanity check: at least 2 breakpoints in window
    if (nb_predicted_breakpoint_indexes < 2){
        return 1;
    }
    
    // sanity check: number of opening breakpoints are equal to number of closing breakpoints
    if ((float)nb_predicted_breakpoint_indexes / 2  != (float)nb_positive_foldchanges){
        return 1;
    }

    // sanity check for alternating pos/neg fold-changes
    bool previous_fold_change = NEGATIVE;                   // assume first fold change in a DUP is positive
    for (size_t it : predicted_breakpoint_indexes){
        if (isPositiveFoldChange[it] != previous_fold_change){
            previous_fold_change = !previous_fold_change;
        }
        else{
            return 1;
        }
    }
    
    // actual breakend reporting
    bool fold_change = POSITIVE;
    for (size_t it : predicted_breakpoint_indexes){
        if (fold_change){
            startPos.push_back(it);
            fold_change = NEGATIVE;
        }
        else{
            endPos.push_back(it);
            fold_change = POSITIVE;
        }
        
    }
    
    return 0;
}


float BreakpointFinder::sd(const std::vector<float> &inData) const{

    float sum = std::accumulate(inData.begin(), inData.end(), 0.0);
    float mean = sum / inData.size();

    std::vector<float> diff(inData.size());
    std::transform(inData.begin(), inData.end(), diff.begin(), [mean](float x) { return x - mean; });
    
    float sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    float stdev = std::sqrt(sq_sum / inData.size());

    return stdev;
}
