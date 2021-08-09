//
//  Match.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#ifndef Match_hpp
#define Match_hpp

#include <stdio.h>
#include "State.hpp"

class Match {
private:
    tuple<int, int, int> indexOfSeekerAndTaker; // the seeker's index, taker's OD index and the taker's index
    double priority; // the priority of the match
    
    double _sharedDistance;
    double _detourDistance;
    double _pickupDistance;
    double _seekerDistance;
    double _takerDistance;
    double _eta;
    void priorityCal(); // calculate the priority, you can change the function to determine how to calculate it

public:
    Match(int indexSeeker, int indexTakerOD, int indexTaker,
          double sharedDistance,
          double detourDistance,
          double pickupDistance,
          double seekerDistance,
          double takerDistance); // initialize the match
    double getPriority(); // get priority
    tuple<int, int, int> getIndexOfSeekerAndTaker(); // get the IndexOfSeekerAndTaker
    void print(); // print the match
    double getSeekerDistance();
    double getTakerDistance();
    double getSharedDistance();
    void setEta(double eta);
    double getEta();
};

#endif /* Match_hpp */
