//
//  NetworkModelCalculate.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/9.
//

#include "Network.hpp"

void Network::iteration()
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
        if (seekerTaker.at(i).size() == 0) {
            seekerStates.at(i).setPSeeker(0);
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
        }
    }
}
