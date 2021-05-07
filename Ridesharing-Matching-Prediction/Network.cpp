//
//  Network.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/6.
//

#include "Network.hpp"
#include <random>

Network::Network(int m, int n,
                 double pickupTime,
                 double maxDetourTime,
                 double searchRadius,
                 double speed)
{
    network = vector<vector<int>>(m, vector<int>(n)); // initialize the network vector
    size = pair<int, int>{m, n}; // initialize the size of the network
    odPairs = vector<OriginDestinationPair>(); // initialize the od pairs vector
    _pickupTime = pickupTime; // initialize the pick up time avg{t^{pk}}
    _maxDetourTime = maxDetourTime; // initialize the max detour time avg{D}
    _searchRadius = searchRadius; // initialize the search radius avg{R}
    _speed = speed; // initialize the speed
}

pair<int, int> Network::positionIndexToPair(int randInt,
                                   int xmin, int ymin, int xmax, int ymax)
{
    int xRange = xmax - xmin + 1;
    int y = ymin + randInt % xRange;
    int x = xmin + randInt / xRange;
    return pair<int, int>{x, y};
}

void Network::generateODPairs(int number, double lambda,
                     int xmin, int ymin, int xmax, int ymax)
{
    int xRange = xmax - xmin + 1;
    int yRange = ymax - ymin + 1;
    int destUniformRandNumRange = xRange * yRange;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> destDistribution(0, destUniformRandNumRange - 1);
    std::uniform_int_distribution<int> originDistribution(0, (size.first * size.second) - 1);
    for (int i = 0; i < number; i++) {
        odPairs.push_back(OriginDestinationPair
                          (positionIndexToPair(originDistribution(generator),
                                               0, 0, size.first-1, size.second-1),
                           positionIndexToPair(destDistribution(generator),
                                               xmin, ymin, xmax, ymax),
                           lambda));
    }
}

void Network::generateSeakerStates()
{
    seakerStates = vector<SeakerState>();
    for (int i = 0; i < odPairs.size(); i++) {
        seakerStates.push_back(SeakerState(odPairs.at(i)));
    }
}

void Network::generateTakerStates()
{
    takerStates = vector<TakerState>();
    for (int i = 0; i < odPairs.size(); i++) {
        auto odLinks = odPairs.at(i).generateLinks();
        for (int j = 0; j < odLinks.size(); j++) {
            takerStates.push_back(TakerState(odLinks.at(j), odPairs.at(i)));
        }
    }
}

void Network::printPairs()
{
    for (int i = 0; i < odPairs.size(); i++) {
        printf("The %dth pair: ", i + 1);
        odPairs.at(i).odPairPrint();
        printf("\n");
    }
}

void Network::printStates()
{
    printf("The seaker states: \n");
    for (int i = 0; i < seakerStates.size(); i++) {
        seakerStates.at(i).printState();
        printf("\n");
    }
    printf("The taker states: \n");
    for (int i = 0; i < takerStates.size(); i++) {
        takerStates.at(i).printState();
        printf("\n");
    }
}
