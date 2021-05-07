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

OriginDestinationPair SeakerState::getODPair()
{
    return _odPair;
}

int TakerState::distanceBetweenNodes(pair<int, int> node1, pair<int, int> node2)
{
    return abs(node1.first - node2.first) + abs(node1.second - node2.second);
}

double TakerState::currentDistanceCal(SeakerState seakerState)
{
    return _link.distanceToNode(seakerState.getODPair().getOrigin());
}

tuple<bool, double, double> TakerState::detourShareDistanceCal(SeakerState seakerState)
{
    double dSeekerFOFO = distanceBetweenNodes(seakerState.getODPair().getOrigin(), _odPair.getDestination()) +
    distanceBetweenNodes(_odPair.getDestination(), seakerState.getODPair().getDestination());
    double dTakerFOFO = _link.distanceToNode(_odPair.getOrigin()) + currentDistanceCal(seakerState) + dSeekerFOFO -
    distanceBetweenNodes(_odPair.getDestination(), seakerState.getODPair().getDestination());
    double dSeekerFOLO = distanceBetweenNodes(seakerState.getODPair().getOrigin(), seakerState.getODPair().getDestination());
    double dTakerFOLO = _link.distanceToNode(_odPair.getOrigin()) + currentDistanceCal(seakerState) + dSeekerFOLO +
    distanceBetweenNodes(_odPair.getDestination(), seakerState.getODPair().getDestination());
    double dSeeker = dSeekerFOLO;
    double dTaker = distanceBetweenNodes(_odPair.getOrigin(), _odPair.getDestination());
    
    double dFOFOMax = (dSeekerFOFO - dSeeker) > (dTakerFOFO - dTaker)? (dSeekerFOFO - dSeeker) : (dTakerFOFO - dTaker);
    double dFOLOMax = (dSeekerFOLO - dSeeker) > (dTakerFOLO - dTaker)? (dSeekerFOLO - dSeeker) : (dTakerFOLO - dTaker);
    return dFOFOMax < dFOLOMax ?
    tuple<bool, double, double>{true, dFOFOMax, distanceBetweenNodes(seakerState.getODPair().getOrigin(), _odPair.getDestination())} :
    tuple<bool, double, double>{false, dFOLOMax, dSeekerFOLO};
}
