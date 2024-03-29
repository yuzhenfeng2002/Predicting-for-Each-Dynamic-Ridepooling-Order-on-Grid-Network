//
//  NetworkInitialize.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/6.
//

#include "Network.hpp"
#include <random>
#include <algorithm>

Network::Network(int m, int n,
                 double pickupTime,
                 double maxDetourTime,
                 double searchRadius,
                 double speed)
{
    size = pair<int, int>{m, n}; // initialize the size of the network
    odPairs = vector<OriginDestinationPair*>(); // initialize the od pairs vector
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

void Network::sortSeekerTaker()
{
    for (int i = 0; i < seekerTaker.size(); i++) {
        sort(seekerTaker[i].begin(), seekerTaker[i].end(), [] (Match *m1, Match *m2)
             {
            return m1->getPriority() > m2->getPriority(); // sort by the priority from larger match to smaller match
        });
    }
}

void Network::generateODPairs(string filePath)
{
    // open csv file
    ifstream inFile(filePath, ios::in);
    if (!inFile)
    {
        printf("Cannot open the file.\n");
    }
    // read csv file
    string singleLine;
    while (getline(inFile, singleLine))
    {
        std::stringstream ss(singleLine);
        string singleWord;
        vector<string> lineArray;
        while (getline(ss, singleWord, ','))
        {
            if (singleWord == "id")
                break;
            lineArray.push_back(singleWord);
        }
        if (singleWord == "id")
            continue; // skip the header
        pair<int, int> origin(atoi(lineArray[1].c_str()), atoi(lineArray[2].c_str()));
        pair<int, int> destination(atoi(lineArray[3].c_str()), atoi(lineArray[4].c_str()));
        double lambda = atof(lineArray[5].c_str());
        odPairs.push_back(new OriginDestinationPair(origin, destination, lambda));
    }
    inFile.close();
}

void Network::generateODPairs(int number, double lambda,
                     int xmin, int ymin, int xmax, int ymax)
{
    int xRange = xmax - xmin + 1;
    int yRange = ymax - ymin + 1;
    int destUniformRandNumRange = xRange * yRange;
    std::default_random_engine generator{0};
    std::uniform_int_distribution<int> destDistribution(0, destUniformRandNumRange - 1);
    std::uniform_int_distribution<int> originDistribution(0, (size.first * size.second) - 1);
    for (int i = 0; i < number; i++) {
        odPairs.push_back(new OriginDestinationPair
                          (positionIndexToPair(originDistribution(generator),
                                               0, 0, size.first-1, size.second-1),
                           positionIndexToPair(destDistribution(generator),
                                               xmin, ymin, xmax, ymax),
                           lambda));
    }
}

void Network::generateSeekerStates()
{
    seekerStates = vector<SeekerState>();
    for (int i = 0; i < odPairs.size(); i++) {
        seekerStates.push_back(SeekerState(odPairs.at(i)));
    }
}

void Network::generateTakerStates()
{
    takerStates = vector<vector<TakerState>>(odPairs.size());
    for (int i = 0; i < odPairs.size(); i++) {
        auto odLinks = odPairs.at(i)->generateLinks();
        for (int j = 0; j < odLinks.size(); j++) {
            takerStates[i].push_back(TakerState(odLinks.at(j), odPairs.at(i), _speed, _pickupTime));
        }
    }
}

void Network::generateMatches()
{
    matches = vector<Match>();
    seekerTaker = vector<vector<Match*>>(seekerStates.size());
    takerSeeker = vector<vector<vector<Match*>>>(takerStates.size());
    for (int i = 0; i < takerStates.size(); i++) {
        takerSeeker.at(i) = vector<vector<Match*>>(takerStates.at(i).size());
    }
    for (int i = 0; i < seekerStates.size(); i++) {
        auto seekerState = seekerStates.at(i);
        for (int j = 0; j < takerStates.size(); j++) {
            for (int k = 0; k < takerStates[j].size(); k++) {
                auto takerState = takerStates.at(j).at(k);
                auto detourShareDistance =
                takerState.detourShareDistanceCal(seekerState);
                auto pickupDistance = takerState.currentDistanceCal(seekerState);
                if (pickupDistance <= _searchRadius) {
                    if (std::get<1>(detourShareDistance) < _maxDetourTime * _speed) {
                        Match *newMatch = new Match(i, j, k,
                                                    std::get<2>(detourShareDistance),
                                                    std::get<1>(detourShareDistance),
                                                    pickupDistance,
                                                    std::get<3>(detourShareDistance),
                                                    std::get<4>(detourShareDistance));
                        matches.push_back(*newMatch);
                        seekerTaker.at(i).push_back(newMatch);
                        takerSeeker.at(j).at(k).push_back(newMatch);
                    }
                }
            }
        }
    }
    sortSeekerTaker();
}

void Network::printPairs(string address)
{
    ofstream file;
    file.open(address);
    file << "id,originX,originY,destX,destY,lambda,Ps,fatherIdx\n";
    for (int i = 0; i < odPairs.size(); i++) {
        auto od = odPairs.at(i);
        auto ss = seekerStates.at(i);
        file << i << ','
        << od->getOrigin().first << ','
        << od->getOrigin().second << ','
        << od->getDestination().first << ','
        << od->getDestination().second << ','
        << od->getLambda() << ','
        << ss.getPSeeker() << ','
        << od->getFatherODIdx() << '\n';
    }
    file.close();
}

void Network::printStates(string address)
{
    ofstream file;
    file.open(address);
    file << "ODid,takerID,fatherIdx,originX,originY,destX,destY,eta,lambda,Pt,rhot\n";
    for (int i = 0; i < takerStates.size(); i++) {
        for (int j = 0; j < takerStates.at(i).size(); j++) {
            auto ts = takerStates.at(i).at(j);
            file << i << ',' << j << ','
            << odPairs.at(i)->getFatherODIdx() << ','
            << ts.getLink().getLink().first.first << ','
            << ts.getLink().getLink().first.second << ','
            << ts.getLink().getLink().second.first << ','
            << ts.getLink().getLink().second.second << ','
            << ts.getEtaTaker() << ','
            << ts.getLambdaTaker() << ','
            << ts.getPTaker() << ','
            << ts.getRhoTaker() << '\n';
        }
    }
    file.close();
}

void Network::printMatches(string address)
{
    for (int i = 0; i < seekerTaker.size(); i++) {
        for (int j = 0; j < seekerTaker.at(i).size(); j++) {
            seekerTaker.at(i).at(j)->print();
        }
    }
}
