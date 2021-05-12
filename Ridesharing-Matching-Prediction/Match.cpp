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

Match::Match(int indexSeeker, int indexTakerOD, int indexTaker,
             double sharedDistance,
             double detourDistance,
             double pickupDistance)
{
    _sharedDistance = sharedDistance;
    _detourDistance = detourDistance;
    _pickupDistance = pickupDistance;
    indexOfSeekerAndTaker = tuple<int, int, int>{indexSeeker, indexTakerOD, indexTaker};
    priorityCal();
}

double Match::getPriority()
{
    return priority;
}

tuple<int, int, int> Match::getIndexOfSeekerAndTaker()
{
    return indexOfSeekerAndTaker;
}

void Match::print()
{
    printf("----------------------------------------------------------\n");
    printf("#Seeker's OD: %d\n#Taker's OD: %d\n#Taker: %d\n", std::get<0>(indexOfSeekerAndTaker), std::get<1>(indexOfSeekerAndTaker), std::get<2>(indexOfSeekerAndTaker));
    printf("#Parameters:\n");
    printf("Pick-up Distance: %f, Detour Distance: %f, Shared Distance: %f\nPriority: %f\n",
           _pickupDistance, _detourDistance, _sharedDistance, priority);
}
