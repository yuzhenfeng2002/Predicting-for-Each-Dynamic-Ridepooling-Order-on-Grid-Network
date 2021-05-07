//
//  Link.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#ifndef Link_hpp
#define Link_hpp

#include <stdio.h>
#include <vector>

using std::pair;

class Link
{
private:
    pair<int, int> _prevNode; // the link's previous node
    pair<int, int> _nextNode; // the link's next node
public:
    Link(pair<int, int> prevNode, pair<int, int> nextNode); // initialize the link
    double distanceToNode(pair<int, int> node); // calculate the distance from the link to a node(approximately)
    void linkPrint(); // print the link
};


#endif /* Link_hpp */
