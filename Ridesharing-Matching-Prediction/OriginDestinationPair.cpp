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

void OriginDestinationPair::ODPairPrint()
{
    printf("Origin: <%d, %d>\tDestination: <%d, %d>\tLambda: %.3f",
           _origin.first, _origin.second,
           _destination.first, _destination.second,
           _lambda);
}
