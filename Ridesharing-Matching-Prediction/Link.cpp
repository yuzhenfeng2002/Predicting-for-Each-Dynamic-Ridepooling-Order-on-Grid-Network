//
//  Link.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/5/7.
//

#include "Link.hpp"

Link::Link(pair<int, int> prevNode, pair<int, int> nextNode)
{
    _prevNode = prevNode;
    _nextNode = nextNode;
}

void Link::linkPrint()
{
    printf("<%d, %d> -> <%d, %d>", _prevNode.first, _prevNode.second, _nextNode.first, _nextNode.second);
}
