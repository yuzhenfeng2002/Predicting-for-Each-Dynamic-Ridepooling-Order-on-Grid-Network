//
//  State.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#include "State.hpp"

SeakerState::SeakerState(OriginDestinationPair odPair) : _odPair(odPair)
{
    ;
}

TakerState::TakerState(Link link, OriginDestinationPair odPair) : _link(link), _odPair(odPair)
{
    ;
}

void SeakerState::printState()
{
    _odPair.odPairPrint();
}

void TakerState::printState()
{
    _odPair.odPairPrint();
    printf("\n");
    _link.linkPrint();
}
