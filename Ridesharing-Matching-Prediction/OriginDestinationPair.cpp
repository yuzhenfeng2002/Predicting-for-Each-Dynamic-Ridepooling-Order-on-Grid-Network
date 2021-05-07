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

vector<Link> OriginDestinationPair::generateLinks()
{
    vector<Link> links = vector<Link>();
    bool isOriginMin = _origin.first < _destination.first;
    if (isOriginMin) {
        for (int i = _origin.first; i < _destination.first; i++) {
            links.push_back(Link(pair<int, int>{i, _origin.second}, pair<int, int>{i + 1, _origin.second}));
        }
    }
    else
    {
        for (int i = _origin.first; i > _destination.first; i--) {
            links.push_back(Link(pair<int, int>{i, _origin.second}, pair<int, int>{i - 1, _origin.second}));
        }
    }
    
    isOriginMin = _origin.second < _destination.second;
    if (isOriginMin) {
        for (int i = _origin.second; i < _destination.second; i++) {
            links.push_back(Link(pair<int, int>{_destination.first, i}, pair<int, int>{_destination.first, i + 1}));
        }
    }
    else
    {
        for (int i = _origin.second; i > _destination.second; i--) {
            links.push_back(Link(pair<int, int>{_destination.first, i}, pair<int, int>{_destination.first, i - 1}));
        }
    }
    return links;
}

void OriginDestinationPair::odPairPrint()
{
    printf("Origin: <%d, %d>\tDestination: <%d, %d>\tLambda: %.3f",
           _origin.first, _origin.second,
           _destination.first, _destination.second,
           _lambda);
}
