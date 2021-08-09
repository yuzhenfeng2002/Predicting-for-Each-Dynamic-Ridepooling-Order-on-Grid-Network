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
    network.generateODPairs(2000, 0.5, 60, 60, 120, 120); // x/y min/max constrain the destination of these OD pairs
    // you can print the OD pairs, the two types of states and the matches here
    clock_t begin_gen = clock();
    network.generateVariables();
    clock_t end_gen = clock();
    std::cout << "The runtime for generating variables is: " << end_gen - begin_gen << " us\n";
    int iteration_time = 0;
    clock_t begin_ite = clock();
    iteration_time = network.iteration(0.01, 0.01); // iteration
    clock_t end_ite = clock();
    // network.calPredictionResult("/Users/fxb/Desktop/拼车预测实验/RESULT_2000.csv");
    std::cout << "The runtime is: " << end_ite - begin_ite << " us\n";
    std::cout << "The iteration number is: " << iteration_time << " \n";
    // network.printPairs("/Users/fxb/Desktop/拼车预测实验/PAIRS_1e4.csv");
    // network.printStates("/Users/fxb/Desktop/拼车预测实验/STATES_1e4.csv");
    network.printResults();
    return 0;
}
