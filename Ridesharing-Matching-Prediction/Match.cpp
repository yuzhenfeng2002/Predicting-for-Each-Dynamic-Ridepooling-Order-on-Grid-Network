//
//  Match.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#include "Match.hpp"

void Match::priorityCal()
{
    const double w1 = 0.5, w2 = 0.3, w3 = 0.2; // the weight for detour, pick-up and shared distance
    priority = - w1 * _detourDistance - w2 * _pickupDistance + w3 * _sharedDistance;
}

Match::Match(int indexSeeker, int indexTaker,
             double sharedDistance,
             double detourDistance,
             double pickupDistance)
{
    _sharedDistance = sharedDistance;
    _detourDistance = detourDistance;
    _pickupDistance = pickupDistance;
    indexOfSeekerAndTaker = pair<int, int>{indexSeeker, indexTaker};
    priorityCal();
}

double Match::getPriority()
{
    return priority;
}

void Match::print()
{
    printf("----------------------------------------------------------\n");
    printf("#Seeker: %d\n#Taker: %d\n", indexOfSeekerAndTaker.first, indexOfSeekerAndTaker.second);
    printf("#Parameters:\n");
    printf("Pick-up Distance: %f, Detour Distance: %f, Shared Distance: %f\nPriority: %f\n",
           _pickupDistance, _detourDistance, _sharedDistance, priority);
}
