//
//  main.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/6.
//

#include <iostream>
#include <ctime>
#include "Network.hpp"

const int MAX_OD_NUM = 20;
int main(int argc, const char * argv[]) {
    for (int i = 0; i < MAX_OD_NUM; i++) {
        Network network = Network(180, 180, 1, 5, 5, 1);
        network.generateODPairs("/Users/fxb/Desktop/误差测试2/OD_Cluster_" + std::to_string(i) + ".csv");
        network.generateVariables();
        int iteration_time = 0;
        iteration_time = network.iteration(0.01, 0.01).first; // iteration
        network.calPredictionResult("/Users/fxb/Desktop/误差测试2/OD_Cluster_Result_" + std::to_string(i) + ".csv");
    }
    return 0;
}
