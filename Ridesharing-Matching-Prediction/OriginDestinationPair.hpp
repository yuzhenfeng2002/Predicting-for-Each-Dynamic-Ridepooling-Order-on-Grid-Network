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
#include "Link.hpp"

using std::pair;
using std::vector;

class OriginDestinationPair {
private:
    pair<int, int> _origin;
    pair<int, int> _destination;
    double _lambda;
public:
    OriginDestinationPair(pair<int, int> origin,
                          pair<int, int> destination,
                          double lambda); // initialize the pair
    vector<Link> generateLinks(); // for each OD pair, generate the links traverse from O to D
    pair<int, int> getOrigin();
    pair<int, int> getDestination();
    void odPairPrint(); // print the pair
};

#endif /* OriginDestinationPair_hpp */
