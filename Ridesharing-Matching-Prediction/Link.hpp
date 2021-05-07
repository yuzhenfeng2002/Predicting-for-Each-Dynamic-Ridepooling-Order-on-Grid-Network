//
//  Link.hpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#ifndef Link_hpp
#define Link_hpp

#include <stdio.h>
class Link
{
private:
    int _prevNode;
    int _nextNode;
public:
    Link(int prevNode, int nextNode); // initialize the link
};


#endif /* Link_hpp */
