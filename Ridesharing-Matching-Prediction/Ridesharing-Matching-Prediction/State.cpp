//
//  State.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#include "State.hpp"

SeekerState::SeekerState(OriginDestinationPair* odPair) : _odPair(odPair)
{
    ;
}

TakerState::TakerState(Link link, OriginDestinationPair* odPair, double speed, double pickupTime) : _link(link), _odPair(odPair)
{
    if (link.getLength() == 0) {
        timeLength = pickupTime;
    }
    else
    {
        timeLength = link.getLength() * 1.0 / speed;
    }
}

void SeekerState::printState()
{
    _odPair->odPairPrint();
}

void TakerState::printState()
{
    _odPair->odPairPrint();
    printf("\n");
    _link.linkPrint();
}

OriginDestinationPair* SeekerState::getODPair()
{
    return _odPair;
}

int TakerState::distanceBetweenNodes(pair<int, int> node1, pair<int, int> node2)
{
    return abs(node1.first - node2.first) + abs(node1.second - node2.second);
}

double TakerState::currentDistanceCal(SeekerState seekerState)
{
    return _link.distanceToNode(seekerState.getODPair()->getOrigin());
}

tuple<bool, double, double, double, double> TakerState::detourShareDistanceCal(SeekerState seekerState)
{
    double dSeekerFOFO = distanceBetweenNodes(seekerState.getODPair()->getOrigin(), _odPair->getDestination()) +
    distanceBetweenNodes(_odPair->getDestination(), seekerState.getODPair()->getDestination());
    double dTakerFOFO = _link.distanceToNode(_odPair->getOrigin()) + currentDistanceCal(seekerState) + dSeekerFOFO -
    distanceBetweenNodes(_odPair->getDestination(), seekerState.getODPair()->getDestination());
    double dSeekerFOLO = distanceBetweenNodes(seekerState.getODPair()->getOrigin(), seekerState.getODPair()->getDestination());
    double dTakerFOLO = _link.distanceToNode(_odPair->getOrigin()) + currentDistanceCal(seekerState) + dSeekerFOLO +
    distanceBetweenNodes(_odPair->getDestination(), seekerState.getODPair()->getDestination());
    double dSeeker = dSeekerFOLO;
    double dTaker = distanceBetweenNodes(_odPair->getOrigin(), _odPair->getDestination());
    
    double dFOFOMax = (dSeekerFOFO - dSeeker) > (dTakerFOFO - dTaker)? (dSeekerFOFO - dSeeker) : (dTakerFOFO - dTaker);
    double dFOLOMax = (dSeekerFOLO - dSeeker) > (dTakerFOLO - dTaker)? (dSeekerFOLO - dSeeker) : (dTakerFOLO - dTaker);
    return dFOFOMax < dFOLOMax ?
    tuple<bool, double, double, double, double>{true, dFOFOMax, distanceBetweenNodes(seekerState.getODPair()->getOrigin(), _odPair->getDestination()), dSeekerFOFO, dTakerFOFO} :
    tuple<bool, double, double, double, double>{false, dFOLOMax, dSeekerFOLO, dSeekerFOLO, dTakerFOLO};
}

double TakerState::getPTaker()
{
    return pTaker;
}
double TakerState::getRhoTaker()
{
    return rhoTaker;
}
double TakerState::getLambdaTaker()
{
    return lambdaTaker;
}
double TakerState::getEtaTaker()
{
    return etaTaker;
}
double TakerState::getTimeLength()
{
    return timeLength;
}
double SeekerState::getPSeeker()
{
    return pSeeker;
}

void TakerState::setPTaker(double p)
{
    pTaker = p;
}
void TakerState::setRhoTaker(double rho)
{
    rhoTaker = rho;
}
void TakerState::setLambdaTaker(double lambda)
{
    lambdaTaker = lambda;
}
void TakerState::setEtaTaker(double eta)
{
    etaTaker = eta;
}
void SeekerState::setPSeeker(double p)
{
    pSeeker = p;
}
