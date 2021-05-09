//
//  Match.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#ifndef Match_hpp
#define Match_hpp

#include <stdio.h>
#include "State.hpp"

class Match {
private:
    pair<int, int> indexOfSeekerAndTaker; // the seeker's and taker's indices
    double priority; // the priority of the match
    
    double _sharedDistance;
    double _detourDistance;
    double _pickupDistance;
    void priorityCal(); // calculate the priority, you can change the function to determine how to calculate it

public:
    Match(int indexSeeker, int indexTaker,
          double sharedDistance,
          double detourDistance,
          double pickupDistance); // initialize the match
    double getPriority(); // get priority
    void print(); // print the match
};

#endif /* Match_hpp */
