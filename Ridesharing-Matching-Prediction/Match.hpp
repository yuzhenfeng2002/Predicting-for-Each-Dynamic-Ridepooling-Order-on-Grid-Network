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
    SeekerState _seekerState; // the seeker of the match
    TakerState _takerState; // the taker of the match
    double priority; // the priority of the match
    
    double _sharedDistance;
    double _detourDistance;
    double _pickupDistance;
    void priorityCal();

public:
    Match(SeekerState seekerState, TakerState takerState,
          double sharedDistance,
          double detourDistance,
          double pickupDistance);
    void print();
};

#endif /* Match_hpp */
