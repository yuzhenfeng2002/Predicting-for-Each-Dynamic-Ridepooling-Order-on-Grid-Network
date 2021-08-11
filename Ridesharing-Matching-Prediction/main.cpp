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
    network.generateODPairs(200, 0.5, 60, 60, 120, 120); // x/y min/max constrain the destination of these OD pairs
    // you can print the OD pairs, the two types of states and the matches here
    auto networks = network.divide();
    
    
    clock_t begin_gen = clock();
    for (int i = 0; i < networks.size(); i++) {
        networks.at(i).generateVariables();
    }
    // if network is not divided
    // network.generateVariables();
    clock_t end_gen = clock();
    std::cout << "The runtime for generating variables is: " << end_gen - begin_gen << " us\n";
    
    
    int iteration_time = 0;
    clock_t begin_ite = clock();
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
    // if network is not divided
    // iteration_time = network.iteration(0.01, 0.01); // iteration
    clock_t end_ite = clock();
    
    
    // if network is not divided
    // network.calPredictionResult("/Users/fxb/Desktop/拼车预测实验/RESULT_200.csv");
    network.combine(networks, "/Users/fxb/Desktop/拼车预测实验/RESULT_2000_combined.csv");
    std::cout << "The runtime is: " << end_ite - begin_ite << " us\n";
    std::cout << "The iteration number is: " << iteration_time << " \n";
    // network.printPairs("/Users/fxb/Desktop/拼车预测实验/PAIRS_1e4.csv");
    // network.printStates("/Users/fxb/Desktop/拼车预测实验/STATES_1e4.csv");
    return 0;
}
