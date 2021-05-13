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
    length = abs(_nextNode.first - _prevNode.first) + abs(_nextNode.second - _prevNode.second);
}

void Link::linkPrint()
{
    printf("<%d, %d> -> <%d, %d>", _prevNode.first, _prevNode.second, _nextNode.first, _nextNode.second);
}

double Link::getLength()
{
    return length;
}

double Link::distanceToNode(pair<int, int> node)
{
    int d1 = abs(node.first - _prevNode.first) + abs(node.second - _prevNode.second);
    int d2 = abs(node.first - _nextNode.first) + abs(node.second - _nextNode.second);
    int d = d1 < d2 ? d1 : d2;
    if (_prevNode.first == _nextNode.first && _prevNode.second == _nextNode.second) {
        return d;
    }
    return d + 0.5; // the dummy link?
}
