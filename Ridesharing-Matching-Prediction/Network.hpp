//
//  Network.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/6.
//

#ifndef Network_hpp
#define Network_hpp

#include <stdio.h>
#include <vector>
#include "OriginDestinationPair.hpp"
#include "State.hpp"
#include "Match.hpp"

using std::vector;

class Network
{
private:
    vector<vector<int>> network;
    pair<int, int> size;
    double _pickupTime;
    double _maxDetourTime;
    double _searchRadius;
    double _speed;
    vector<OriginDestinationPair> odPairs;
    vector<SeekerState> seekerStates;
    vector<vector<TakerState>> takerStates;
    vector<vector<Match>> seekerTaker;
    vector<vector<Match>> takerSeeker;
    vector<Match> matches;
    
    

    void sortSeekerTaker(); // sort the match of a seeker according to its priority
    void generateSeekerStates(); // generate all the seeker states
    void generateTakerStates(); // generate all the taker states
    void generateMatches(); // generate all the matches s.t. the two conditions
    pair<int, int> positionIndexToPair(int randInt,
                                       int xmin, int ymin, int xmax, int ymax); // turn the generated rand int to position pair constained in a rectangle ranging from (xmin, ymin) to (xmax, ymax)
    tuple<double, double, double, double> iterationStep(); // one iteration step
public:
    Network(int m, int n,
            double pickupTime,
            double maxDetourTime,
            double searchRadius,
            double speed); // initialize a m*n network
    void generateODPairs(int number, double lambda,
                         int xmin, int ymin, int xmax, int ymax); // generate {number}s OD pairs and constain *destinations* in a rectangle ranging from (xmin, ymin) to (xmax, ymax)
    
    void printPairs(); // print all the pairs
    void printStates(); // print all the seeker and taker states
    void printMatches(); // print all the matches
    void iteration(double lambdaEpsilon, double probabilityEpsilon, int iterationTime = -1);
};

#endif /* Network_hpp */
