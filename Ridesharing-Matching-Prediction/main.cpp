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
    /* Network::Network(int m, int n,
                     double pickupTime,
                     double maxDetourTime,
                     double searchRadius,
                     double speed) */
    Network network = Network(30, 30, 2, 2, 2, 1);
    network.generateODPairs(50, 0.5, 10, 10, 20, 20);
//    network.printMatches();
//    network.printPairs();
//    network.printStates();
    clock_t begin = clock();
    network.iteration(0.05, 0.005, 5);
    clock_t end = clock();
    std::cout << "The runtime is: " << end - begin << " us\n";
    return 0;
}
