//
//  OriginDestinationPair.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/6.
//

#ifndef OriginDestinationPair_hpp
#define OriginDestinationPair_hpp

#include <stdio.h>
#include <vector>

using std::pair;

class OriginDestinationPair {
private:
    pair<int, int> _origin;
    pair<int, int> _destination;
    double _lambda;
public:
    OriginDestinationPair(pair<int, int> origin,
                          pair<int, int> destination,
                          double lambda); // initialize the pair
    void ODPairPrint(); // print the pair
};

#endif /* OriginDestinationPair_hpp */
