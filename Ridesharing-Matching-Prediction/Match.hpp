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
    SeakerState _seakerState; // the seeker of the match
    TakerState _takerState; // the taker of the match
    double priority; // the priority of the match
    
    double sharedDistance;
    double detourDistance;
    double pickupDistance;

public:
    Match(SeakerState seakerState, TakerState takerState,
          double sharedDistance,
          double detourDistance,
          double pickupDistance);
};

#endif /* Match_hpp */
