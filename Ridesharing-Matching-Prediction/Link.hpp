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
    pair<int, int> _prevNode;
    pair<int, int> _nextNode;
public:
    Link(pair<int, int> prevNode, pair<int, int> nextNode); // initialize the link
    void linkPrint();
};


#endif /* Link_hpp */
