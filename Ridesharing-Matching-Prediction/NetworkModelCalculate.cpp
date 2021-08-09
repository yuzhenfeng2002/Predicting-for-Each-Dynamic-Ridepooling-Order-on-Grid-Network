//
//  NetworkModelCalculate.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/9.
//

#include <algorithm>
#include "Network.hpp"

tuple<double, double, double, double> Network::iterationStep(double lambdaEpsilon, double probabilityEpsilon)
{
    vector<double> lambdaStep = vector<double>();
    vector<double> pSeekerStep = vector<double>();
    vector<double> pTakerStep = vector<double>();
    vector<double> rhoTakerStep = vector<double>();
    for (int i = 0; i < takerStates.size(); i++) {
        for (int j = 0; j < takerStates.at(i).size(); j++) {
            if (j == 0) {
                auto lambdaTaker = odPairs.at(i).getLambda() * (1 - seekerStates.at(i).getPSeeker());
                lambdaStep.push_back(abs(lambdaTaker - takerStates.at(i).at(j).getLambdaTaker()));
                takerStates.at(i).at(j).setLambdaTaker(lambdaTaker);
            }
            else
            {
                auto lambdaTaker = takerStates.at(i).at(j - 1).getLambdaTaker() * (1 - takerStates.at(i).at(j - 1).getPTaker());
                lambdaStep.push_back(abs(lambdaTaker - takerStates.at(i).at(j).getLambdaTaker()));
                takerStates.at(i).at(j).setLambdaTaker(lambdaTaker);
            }
        }
    }
    
    for (int i = 0; i < seekerStates.size(); i++) {
        double pSeeker;
        if (seekerTaker.at(i).size() == 0) {
            pSeeker = 0;
        }
        else
        {
            double multiplyResult = 1;
            for (int j = 0; j < seekerTaker.at(i).size(); j++) {
                auto index = (*seekerTaker.at(i).at(j)).getIndexOfSeekerAndTaker();
                int indexOD = std::get<1>(index);
                int indexTaker = std::get<2>(index);
                multiplyResult *= 1 - takerStates.at(indexOD).at(indexTaker).getRhoTaker();
            }
            pSeeker = 1 - multiplyResult;
        }
        pSeekerStep.push_back(abs(pSeeker - seekerStates.at(i).getPSeeker()));
        seekerStates.at(i).setPSeeker(pSeeker);
    }
    
    for (int i = 0; i < takerStates.size(); i++) {
        for (int j = 0; j < takerStates.at(i).size(); j++) {
            double pTaker = 1 - exp(
                                    -takerStates.at(i).at(j).getEtaTaker() *
                                    takerStates.at(i).at(j).getTimeLength());
            pTakerStep.push_back(abs(takerStates.at(i).at(j).getPTaker() - pTaker));
            takerStates.at(i).at(j).setPTaker(pTaker);
        }
    }
    
    for (int i = 0; i < takerStates.size(); i++) {
        for (int j = 0; j < takerStates.at(i).size(); j++) {
            double rhoTaker;
            if (takerStates.at(i).at(j).getEtaTaker() == 0) {
                rhoTaker = takerStates.at(i).at(j).getLambdaTaker() *
                takerStates.at(i).at(j).getTimeLength();
            }
            else
            {
                rhoTaker = 1 - exp(
                                   -takerStates.at(i).at(j).getEtaTaker() *
                                   takerStates.at(i).at(j).getTimeLength());
                rhoTaker = rhoTaker * takerStates.at(i).at(j).getLambdaTaker() /
                takerStates.at(i).at(j).getEtaTaker();
            }
            rhoTakerStep.push_back(abs(takerStates.at(i).at(j).getRhoTaker() - rhoTaker));
            takerStates.at(i).at(j).setRhoTaker(rhoTaker);
        }
    }
    
    // calculate the eta of each taker state
    for (int i = 0; i < takerStates.size(); i++) {
        for (int j = 0; j < takerStates.at(i).size(); j++) {
            takerStates.at(i).at(j).setEtaTaker(0);
        }
    }
    
    for (int i = 0; i < seekerTaker.size(); i++) {
        double lambda = odPairs.at(i).getLambda();
        for (int j = 0; j < seekerTaker.at(i).size(); j++) {
            auto index = seekerTaker.at(i).at(j)->getIndexOfSeekerAndTaker();
            int indexOD = std::get<1>(index);
            int indexTaker = std::get<2>(index);
            lambda *= (1 - takerStates.at(indexOD).at(indexTaker).getRhoTaker());
            seekerTaker.at(i).at(j)->setEta(lambda);
            takerStates.at(indexOD).at(indexTaker).setEtaTaker(takerStates.at(indexOD).at(indexTaker).getEtaTaker() + lambda);
        }
    }
    
    return tuple<double, double, double, double>{
        *std::max_element(lambdaStep.begin(), lambdaStep.end()),
        *std::max_element(rhoTakerStep.begin(), rhoTakerStep.end()),
        *std::max_element(pTakerStep.begin(), pTakerStep.end()),
        *std::max_element(pSeekerStep.begin(), pSeekerStep.end())};
}

void Network::generateVariables()
{
    generateSeekerStates(); // generate all the seeker states
    generateTakerStates(); // generate all the taker states
    generateMatches(); // generate all the matches
    // note that when there are several groups of OD pairs, these three functions needs to be called after all the OD pairs are generated
}

int Network::iteration(double lambdaEpsilon, double probabilityEpsilon, int iterationTime)
{
    int time = 0;
    bool willContinue = true;
    double lambdaStep, rhoTakerStep, pTakerStep, pSeekerStep;
    printf("time,lambda,rho,p_t,p_s\n");
    do {
        auto iterationResult = iterationStep(lambdaEpsilon, probabilityEpsilon);
        lambdaStep = std::get<0>(iterationResult);
        rhoTakerStep = std::get<1>(iterationResult);
        pTakerStep = std::get<2>(iterationResult);
        pSeekerStep = std::get<3>(iterationResult);
        time ++;
        printf("%d,%f,%f,%f,%f\n", time, lambdaStep, rhoTakerStep, pTakerStep, pSeekerStep);
        if (iterationTime == -1) {
            willContinue = true;
        }
        else if (time < iterationTime)
        {
            willContinue = true;
        }
        else
        {
            willContinue = false;
        }
    } while (willContinue &&
             (lambdaStep > lambdaEpsilon ||
             rhoTakerStep > probabilityEpsilon ||
             pTakerStep > probabilityEpsilon ||
             pSeekerStep > probabilityEpsilon));
    return time;
}

int distanceBetweenNodes(pair<int, int> node1, pair<int, int> node2)
{
    return abs(node1.first - node2.first) + abs(node1.second - node2.second);
}

void Network::calPredictionResult(string address)
{
    ofstream file;
    file.open(address);
    file << "id,originX,originY,destX,destY,lambda,Pw,Lw,ew\n";
    for (int i = 0; i < odPairs.size(); i++) {
        double pw = 1 - takerStates.at(i).at(takerStates.at(i).size() - 1).getLambdaTaker() / odPairs.at(i).getLambda();
        
        double ls_numerator = 0;
        for (int k = 0; k < seekerTaker.at(i).size(); k++) {
            auto index = seekerTaker.at(i).at(k)->getIndexOfSeekerAndTaker();
            int indexOD = std::get<1>(index);
            int indexTaker = std::get<2>(index);
            ls_numerator += seekerTaker.at(i).at(k)->getEta() * takerStates.at(indexOD).at(indexTaker).getRhoTaker() * seekerTaker.at(i).at(k)->getSeekerDistance();
        }
        
        double lw = ls_numerator * 1.0 / odPairs.at(i).getLambda() + (1 - pw) * distanceBetweenNodes(odPairs.at(i).getOrigin(), odPairs.at(i).getDestination());
        for (int j = 0; j < takerStates.at(i).size(); j++) {
            double lt_numerator = 0;
            double lt_denominator = 0;
            for (int k = 0; k < takerSeeker.at(i).at(j).size(); k++) {
                auto index = takerSeeker.at(i).at(j).at(k)->getIndexOfSeekerAndTaker();
                int indexOD = std::get<1>(index);
                int indexTaker = std::get<2>(index);
                lt_numerator += takerSeeker.at(i).at(j).at(k)->getEta() * takerStates.at(indexOD).at(indexTaker).getRhoTaker() * takerSeeker.at(i).at(j).at(k)->getTakerDistance();
                lt_denominator += takerSeeker.at(i).at(j).at(k)->getEta() * takerStates.at(indexOD).at(indexTaker).getRhoTaker();
            }
            if (lt_denominator != 0) {
                lw += takerStates.at(i).at(j).getLambdaTaker() * lt_numerator / lt_denominator / odPairs.at(i).getLambda();
            }
        }
        
        double es_numerator = 0;
        for (int k = 0; k < seekerTaker.at(i).size(); k++) {
            auto index = seekerTaker.at(i).at(k)->getIndexOfSeekerAndTaker();
            int indexOD = std::get<1>(index);
            int indexTaker = std::get<2>(index);
            es_numerator += seekerTaker.at(i).at(k)->getEta() * takerStates.at(indexOD).at(indexTaker).getRhoTaker() * seekerTaker.at(i).at(k)->getSharedDistance();
        }
        
        double ew = es_numerator * 1.0 / odPairs.at(i).getLambda();
        for (int j = 0; j < takerStates.at(i).size(); j++) {
            double et_numerator = 0;
            double et_denominator = 0;
            for (int k = 0; k < takerSeeker.at(i).at(j).size(); k++) {
                auto index = takerSeeker.at(i).at(j).at(k)->getIndexOfSeekerAndTaker();
                int indexOD = std::get<1>(index);
                int indexTaker = std::get<2>(index);
                et_numerator += takerSeeker.at(i).at(j).at(k)->getEta() * takerStates.at(indexOD).at(indexTaker).getRhoTaker() * takerSeeker.at(i).at(j).at(k)->getSharedDistance();
                et_denominator += takerSeeker.at(i).at(j).at(k)->getEta() * takerStates.at(indexOD).at(indexTaker).getRhoTaker();
            }
            if (et_denominator != 0) {
                ew += takerStates.at(i).at(j).getLambdaTaker() * et_numerator / et_denominator / odPairs.at(i).getLambda();
            }
        }
        
        auto od = odPairs.at(i);
        file << i << ','
        << od.getOrigin().first << ','
        << od.getOrigin().second << ','
        << od.getDestination().first << ','
        << od.getDestination().second << ','
        << od.getLambda() << ','
        << pw << ',' << lw << ',' << ew << '\n';
    }
    file.close();
}

void Network::printResults()
{
    printf("The result of the model is: \n");
    
    printf("The size of the network is %d * %d. \n", size.first, size.second);
    printf("The speed, pick-up time, maximum detour time and search radius of the network is %.1f, %.1f, %.1f, %.1f. \n",
           _speed, _pickupTime, _maxDetourTime, _searchRadius);
    
    printf("There are %lu numbers of OD pairs. \n", odPairs.size());
    
    int sum = 0;
    for (int i = 0; i < takerStates.size(); i++) {
        sum += takerStates.at(i).size();
    }
    
    printf("There are %d numbers of taker states. \n", sum);
    printf("There are %lu numbers of matches. \n", matches.size());
}
