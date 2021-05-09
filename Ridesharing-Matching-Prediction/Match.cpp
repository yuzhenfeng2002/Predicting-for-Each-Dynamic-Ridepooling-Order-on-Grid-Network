//
//  Match.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#include "Match.hpp"

void Match::priorityCal()
{
    const int w1 = 0.5, w2 = 0.3, w3 = 0.2; // the weight for detour, pick-up and shared distance
    priority = w1 * _detourDistance + w2 * _pickupDistance + w3 * _sharedDistance;
}

Match::Match(SeekerState seekerState, TakerState takerState,
             double sharedDistance,
             double detourDistance,
             double pickupDistance) : _seekerState(seekerState), _takerState(takerState)
{
    _sharedDistance = sharedDistance;
    _detourDistance = detourDistance;
    _pickupDistance = pickupDistance;
    priorityCal();
}

void Match::print()
{
    printf("----------------------------------------------------------\n");
    printf("#Taker:\n");
    _takerState.printState();
    printf("\n");
    printf("#Seeker:\n");
    _seekerState.printState();
    printf("\n");
    printf("#Parameters:\n");
    printf("Pick-up Distance: %f, Detour Distance: %f, Shared Distance: %f\n",
           _pickupDistance, _detourDistance, _sharedDistance);
}
