//
//  main.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/6.
//

#include <iostream>
#include <ctime>
#include "Network.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    /*
     Network(int m, int n,
                     double pickupTime,
                     double maxDetourTime,
                     double searchRadius,
                     double speed)
     void generateODPairs(int number, double lambda,
                          int xmin, int ymin, int xmax, int ymax);
    */
    Network network = Network(180, 180, 1, 5, 5, 1);
//    network.generateODPairs(1000, 0.2, 60, 60, 120, 120); // x/y min/max constrain the destination of these OD pairs
//    network.generateODPairs(500, 0.1, 120, 120, 180, 180);
//    network.generateODPairs(500, 0.05, 0, 0, 180, 180);
//    network.generateODPairs(200, 0.5, 60, 60, 120, 120);
    network.generateODPairs("/Users/fxb/Desktop/拼车预测实验/OD_200.csv");
    
    // you can print the OD pairs, the two types of states and the matches here
    auto networks = network.divide();
    
    
    clock_t begin_gen = clock();
    // if network is divided
    /*
    for (int i = 0; i < networks.size(); i++) {
        networks.at(i).generateVariables();
    }
    */
    // if network is not divided
    network.generateVariables();
    clock_t end_gen = clock();
    std::cout << "The runtime for generating variables is: " << end_gen - begin_gen << " us\n";
    
    
    int iteration_time = 0;
    clock_t begin_ite = clock();
    // if network is divided
    /*
    for (int k = 0; k < 100; k++) {
        double max = 0;
        for (int i = 0; i < networks.size(); i++) {
            auto ite_result = networks.at(i).iteration(0.01, 0.01, 200);
            iteration_time += ite_result.first;
            if (ite_result.second > max) {
                max = ite_result.second;
            }
        }
        if (max < 1e-6) {
            break;
        }
        // std::cout << max << std::endl;
    }
    */
    // if network is not divided
    iteration_time = network.iteration(0.01, 0.01).first; // iteration
    clock_t end_ite = clock();
    
    std::cout << "The runtime is: " << end_ite - begin_ite << " us\n";
    std::cout << "The iteration number is: " << iteration_time << " \n";
    
    // network.printMatches("/Users/fxb/Desktop/拼车预测实验/MATCH_200.csv");
    // if network is not divided
    network.calPredictionResult("/Users/fxb/Desktop/拼车预测实验/RESULT_200.csv");
    
    // if network is divided
    // network.combine(networks, "/Users/fxb/Desktop/拼车预测实验/RESULT_200_combined.csv");

    /*
    for (int i = 0; i < networks.size(); i++) {
        networks.at(i).printPairs("/Users/fxb/Desktop/拼车预测实验/PAIRS_200_" + std::to_string(i) + ".csv");
        networks.at(i).printStates("/Users/fxb/Desktop/拼车预测实验/STATES_200_" + std::to_string(i) + ".csv");
    }
    */
    network.printPairs("/Users/fxb/Desktop/拼车预测实验/PAIRS_200.csv");
    network.printStates("/Users/fxb/Desktop/拼车预测实验/STATES_200.csv");
    return 0;
}
