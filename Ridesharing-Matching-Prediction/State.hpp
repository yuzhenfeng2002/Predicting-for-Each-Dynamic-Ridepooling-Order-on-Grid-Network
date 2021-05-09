//
//  State.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#ifndef State_hpp
#define State_hpp

#include <stdio.h>
#include <vector>
#include "Link.hpp"
#include "OriginDestinationPair.hpp"

using std::tuple;

class SeekerState {
private:
    OriginDestinationPair _odPair;
    double pSeeker = 0.5;
public:
    SeekerState(OriginDestinationPair odPair); // initialize the seeker state
    OriginDestinationPair getODPair();
    void printState();
    
    void setPSeeker(double p);
    double getPSeeker();
};

class TakerState
{
private:
    Link _link;
    OriginDestinationPair _odPair;
    double pTaker = 0.5;
    double rhoTaker = 0.5;
    double lambdaTaker = 0;
    int distanceBetweenNodes(pair<int, int> node1, pair<int, int> node2); // calculate the distance between two nodes
public:
    TakerState(Link link, OriginDestinationPair odPair); // initialize the taker state
    double currentDistanceCal(SeekerState seekerState); // calculate the current distance between the taker and the seeker, namely pick-up distance
    tuple<bool, double, double> detourShareDistanceCal(SeekerState seekerState); // return(isFOFO, detour distance, share distance)
    void printState();
    
    void setPTaker(double p);
    double getPTaker();
    void setRhoTaker(double p);
    double getRhoTaker();
    void setLambdaTaker(double p);
    double getLambdaTaker();
};
#endif /* State_hpp */
