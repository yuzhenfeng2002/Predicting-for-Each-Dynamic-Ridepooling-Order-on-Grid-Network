//
//  NetworkModelCalculate.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/9.
//

#include <algorithm>
#include "Network.hpp"

tuple<double, double, double, double> Network::iterationStep()
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
                auto index = seekerTaker.at(i).at(j).getIndexOfSeekerAndTaker();
                int indexOD = std::get<1>(index);
                int indexTaker = std::get<2>(index);
                multiplyResult *= takerStates.at(indexOD).at(indexTaker).getRhoTaker();
            }
            pSeeker = 1 - multiplyResult;
        }
        pSeekerStep.push_back(abs(pSeeker - seekerStates.at(i).getPSeeker()));
        seekerStates.at(i).setPSeeker(pSeeker);
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
            auto index = seekerTaker.at(i).at(j).getIndexOfSeekerAndTaker();
            int indexOD = std::get<1>(index);
            int indexTaker = std::get<2>(index);
            lambda *= (1 - takerStates.at(indexOD).at(indexTaker).getRhoTaker());
            takerStates.at(indexOD).at(indexTaker).setEtaTaker(takerStates.at(indexOD).at(indexTaker).getEtaTaker() + lambda);
        }
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
    
    for (int i = 0; i < seekerTaker.size(); i++) {
        for (int j = int(seekerTaker.at(i).size()) - 1; j >= 0; j--) {
            auto index = seekerTaker.at(i).at(j).getIndexOfSeekerAndTaker();
            int indexOD = std::get<1>(index);
            int indexTaker = std::get<2>(index);
            double rhoTaker;
            if (takerStates.at(indexOD).at(indexTaker).getEtaTaker() == 0) {
                rhoTaker = takerStates.at(indexOD).at(indexTaker).getLambdaTaker() *
                takerStates.at(indexOD).at(indexTaker).getTimeLength();
            }
            else
            {
                rhoTaker = 1 - exp(
                                   -takerStates.at(indexOD).at(indexTaker).getEtaTaker() *
                                   takerStates.at(indexOD).at(indexTaker).getTimeLength());
                rhoTaker = rhoTaker * takerStates.at(indexOD).at(indexTaker).getLambdaTaker() /
                takerStates.at(indexOD).at(indexTaker).getEtaTaker();
            }
            rhoTakerStep.push_back(abs(takerStates.at(indexOD).at(indexTaker).getRhoTaker() - rhoTaker));
            takerStates.at(indexOD).at(indexTaker).setRhoTaker(rhoTaker);
        }
    }
    
    return tuple<double, double, double, double>{
        *std::max_element(lambdaStep.begin(), lambdaStep.end()),
        *std::max_element(rhoTakerStep.begin(), rhoTakerStep.end()),
        *std::max_element(pTakerStep.begin(), pTakerStep.end()),
        *std::max_element(pSeekerStep.begin(), pSeekerStep.end())};
}

void Network::iteration(double lambdaEpsilon, double probabilityEpsilon, int iterationTime)
{
    int time = 0;
    double lambdaStep, rhoTakerStep, pTakerStep, pSeekerStep;
    do {
        auto iterationResult = iterationStep();
        lambdaStep = std::get<0>(iterationResult);
        rhoTakerStep = std::get<1>(iterationResult);
        pTakerStep = std::get<2>(iterationResult);
        pSeekerStep = std::get<3>(iterationResult);
        time ++;
        printf("------------------------\n");
        printf("The %dth time iteration:\n", time);
        printf("lambdaStep = %f\n", lambdaStep);
        printf("rhoTakerStep = %f\n", rhoTakerStep);
        printf("pTakerStep = %f\n", pTakerStep);
        printf("pSeekerStep = %f\n", pSeekerStep);
    } while (time != iterationTime ||
             lambdaStep > lambdaEpsilon ||
             rhoTakerStep > probabilityEpsilon ||
             pTakerStep > probabilityEpsilon ||
             pSeekerStep > probabilityEpsilon);
}
