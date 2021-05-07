//
//  State.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#ifndef State_hpp
#define State_hpp

#include <stdio.h>
#include "Link.hpp"
#include "OriginDestinationPair.hpp"

class TakerState
{
private:
    Link _link;
    OriginDestinationPair _odPair;
public:
    TakerState(Link link, OriginDestinationPair odPair); // initialize the taker state
};
class SeakerState {
private:
    OriginDestinationPair _odPair;
public:
    SeakerState(OriginDestinationPair odPair); // initialize the seaker state
};
#endif /* State_hpp */
