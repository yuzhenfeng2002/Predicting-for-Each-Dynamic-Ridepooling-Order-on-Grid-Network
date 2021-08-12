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
    OriginDestinationPair *_pre_sub_od = NULL;
    OriginDestinationPair *_nxt_sub_od = NULL;
    int father_od_idx = -1;
    double _lambda;
public:
    OriginDestinationPair(pair<int, int> origin,
                          pair<int, int> destination,
                          double lambda); // initialize the pair
    vector<Link> generateLinks(); // for each OD pair, generate the links traverse from O to D
    pair<int, int> getOrigin(); // get the position of origin
    pair<int, int> getDestination(); // get the position of destination
    double getLambda(); // get lambda of the OD
    void odPairPrint(); // print the pair
    void setPreSubOD(OriginDestinationPair *pre_sub_od);
    void setNextSubOD(OriginDestinationPair *nxt_sub_od){_nxt_sub_od = nxt_sub_od;}
    void setFatherODIdx(int idx);
    int getFatherODIdx();
    OriginDestinationPair* getNextSubOD(){return _nxt_sub_od;}
    OriginDestinationPair* getPreSubOD(){return _pre_sub_od;}
    void setLambda(double lambda){_lambda = lambda;}
};

#endif /* OriginDestinationPair_hpp */
