//
//  OriginDestinationPair.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/6.
//

#include "OriginDestinationPair.hpp"

OriginDestinationPair::OriginDestinationPair(pair<int, int> origin,
                      pair<int, int> destination,
                      double lambda)
{
    _origin = origin;
    _destination = destination;
    _lambda = lambda;
}
