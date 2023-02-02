#include "BreakpointFinder.h"


void BreakpointFinder::setThreshold(const float threshold){

    this->threshold_ = threshold;

}


void BreakpointFinder::setThreshold(const std::vector<float> &inData){

    const float sd = this->sd(inData);

    this->threshold_ = std::max(3 * sd, 0.2f);

}


float BreakpointFinder::getThreshold() const{
        return this->threshold_;
}


int BreakpointFinder::findBreakpoints(std::vector<unsigned> &startPos, std::vector<unsigned> &endPos, const std::vector<float> &inData) const{

    std::vector<bool> significantPositiveFoldChanges(inData.size(), false);
    std::vector<bool> significantNegativeFoldChanges(inData.size(), false);
    unsigned nb_significant_positive_foldchanges = 0;
    unsigned nb_significant_negative_foldchanges = 0;
    
    // TODO: for now we only consider non-nested (tandem) duplications being present in the window under investigation
    // TODO: implement a rescue/warning/report mechanism here if the breakpoint pattern does not indicate consecutive DUP / other artifacts

    for (size_t idx = 1; idx < inData.size(); ++idx){         // idx=0 == '0' because of the pairwise normalization in the window
        
        if(geqt(inData[idx])){
            significantPositiveFoldChanges[idx] = true;
            ++nb_significant_positive_foldchanges;
        }

        if (leqt(inData[idx])){
            significantNegativeFoldChanges[idx] = true;
            ++nb_significant_negative_foldchanges;
        }

    }

    // sanity check: at least 2 breakpoints in window
    if ((nb_significant_positive_foldchanges + nb_significant_negative_foldchanges) < 2){
        fprintf(stdout, "[BreakpointFinder] There are less than two predicted breakpoints in the window.\n");
        return 1;
    }
    
    // sanity check: number of opening breakpoints are equal to number of closing breakpoints
    if (nb_significant_negative_foldchanges != nb_significant_positive_foldchanges){
        fprintf(stdout, "[BreakpointFinder] Number of positive and negative coverage foldchanges are not equal.\n");
        return 2;
    }

    // sanity check for DUP pattern here
    bool is_dup_pattern = verify_DUP_pattern(significantPositiveFoldChanges, significantNegativeFoldChanges);


    // TODO: sanity check for DEL pattern here


    // augment return vectors with window indexes
    if(is_dup_pattern){
        for (unsigned i=0 ; i < significantPositiveFoldChanges.size(); ++i){
            if (significantPositiveFoldChanges[i]){
                startPos.push_back(i);
            }
            
            if (significantNegativeFoldChanges[i]){
                endPos.push_back(i);
            }
            
        }
    }
    
    return 0;
}


inline bool BreakpointFinder::verify_DUP_pattern(const std::vector<bool> &posChanges, const std::vector<bool> &negChanges) const{

    int stack = 0;

    assert(posChanges.size() == negChanges.size());

    for (size_t i = 0; i < posChanges.size(); ++i){
        
        if (posChanges[i]){
            ++stack;
        }
        if (negChanges[i]){
            --stack;
        }
        
        if (stack < 0){
            return false;
        }
        
    }

    assert(stack == 0); // neg values would have failed in loop above; pos values are caught in outer scope before this function call
    
    return true;
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
