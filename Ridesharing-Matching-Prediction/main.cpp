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
    Network network = Network(30, 30, 2, 2, 2, 1);
    network.generateODPairs(50, 0.5, 10, 10, 20, 20); // x/y min/max constrain the destination of these OD pairs
    // you can print the OD pairs, the two types of states and the matches here
    // network.printMatches();
    // network.printPairs();
    // network.printStates();
    clock_t begin = clock();
    network.iteration(0.0005, 0.00005); // iteration
    clock_t end = clock();
    std::cout << "The runtime is: " << end - begin << " us\n";
    return 0;
}
