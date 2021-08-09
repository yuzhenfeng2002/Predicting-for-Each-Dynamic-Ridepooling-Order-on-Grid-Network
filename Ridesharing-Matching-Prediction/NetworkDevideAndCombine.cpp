//
//  NetworkDevideAndCombine.cpp
//  Ridesharing-Matching-Prediction
//
//  Created by YzFENG on 2021/8/8.
//

#include "Network.hpp"

int calSubIndex(pair<int, int> node, pair<int, int> size)
{
    if (node.first <= size.first / 2) {
        if (node.second <= size.second / 2) {
            return 1;
        }
        else
            return 2;
    }
    else
    {
        if (node.second <= size.second / 2) {
            return 3;
        }
        else
            return 4;
    }
}

vector<Network> Network::divide()
{
    vector<Network> subNetworks = vector<Network>(4, Network(size.first/2, size.second/2, _pickupTime, _maxDetourTime, _searchRadius, _speed));
    for (int i = 0; i < odPairs.size(); i++) {
        auto od = odPairs.at(i);
        od.setFatherODIdx(i);
        auto origin = od.getOrigin();
        auto dest = od.getDestination();
        int origin_sub_idx = calSubIndex(origin, size) - 1;
        int dest_sub_idx = calSubIndex(dest, size) - 1;
        if (origin_sub_idx == dest_sub_idx) {
            subNetworks.at(origin_sub_idx).odPairs.push_back(od);
            continue;
        }
        auto mid_node = pair<int, int>{dest.first, origin.second};
        int mid_node_sub_idx = calSubIndex(mid_node, size) - 1;
        if (mid_node_sub_idx == origin_sub_idx)
        {
            auto pre_od = OriginDestinationPair(origin, pair<int, int>{mid_node.first, size.second/2}, od.getLambda());
            pre_od.setFatherODIdx(i);
            subNetworks.at(origin_sub_idx).odPairs.push_back(pre_od);
            auto next_od = OriginDestinationPair(pair<int, int>{mid_node.first, size.second/2}, dest, 1e-6);
            next_od.setPreSubOD(&pre_od);
            next_od.setFatherODIdx(i);
            subNetworks.at(dest_sub_idx).odPairs.push_back(next_od);
        }
        else if (mid_node_sub_idx == dest_sub_idx)
        {
            auto pre_od = OriginDestinationPair(origin, pair<int, int>{size.first/2, mid_node.second}, od.getLambda());
            pre_od.setFatherODIdx(i);
            subNetworks.at(origin_sub_idx).odPairs.push_back(pre_od);
            auto next_od = OriginDestinationPair(pair<int, int>{size.first/2, mid_node.second}, dest, 1e-6);
            next_od.setPreSubOD(&pre_od);
            next_od.setFatherODIdx(i);
            subNetworks.at(dest_sub_idx).odPairs.push_back(next_od);
        }
        else
        {
            auto pre_od = OriginDestinationPair(origin, pair<int, int>{size.first/2, mid_node.second}, od.getLambda());
            pre_od.setFatherODIdx(i);
            subNetworks.at(origin_sub_idx).odPairs.push_back(pre_od);
            auto mid_od = OriginDestinationPair(pair<int, int>{size.first/2, mid_node.second}, pair<int, int>{mid_node.first, size.second/2}, 1e-6);
            mid_od.setPreSubOD(&pre_od);
            mid_od.setFatherODIdx(i);
            subNetworks.at(mid_node_sub_idx).odPairs.push_back(mid_od);
            auto next_od = OriginDestinationPair(pair<int, int>{mid_node.first, size.second/2}, dest, 1e-6);
            next_od.setPreSubOD(&mid_od);
            next_od.setFatherODIdx(i);
            subNetworks.at(dest_sub_idx).odPairs.push_back(next_od);
        }
    }
    return subNetworks;
}

vector<tuple<double, double, double>> Network::combine(vector<Network> networks, string address)
{
    auto result = vector<tuple<double, double, double>>(odPairs.size(), tuple<double, double, double>{1, 0, 0});
    
    for (int i = 0; i < networks.size(); i++) {
        auto sub_result = networks.at(i).calPredictionResult();
        for (int j = 0; j < sub_result.size(); j++) {
            int father_idx = networks.at(i).odPairs.at(j).getFatherODIdx();
            auto sub_pw = std::get<0>(sub_result.at(j));
            auto sub_lw = std::get<1>(sub_result.at(j));
            auto sub_ew = std::get<2>(sub_result.at(j));
            std::get<0>(result.at(father_idx)) *= 1 - sub_pw;
            std::get<1>(result.at(father_idx)) += sub_lw;
            std::get<2>(result.at(father_idx)) += sub_ew;
        }
    }
    
    if (address != "")
    {
        ofstream file;
        file.open(address);
        file << "id,originX,originY,destX,destY,lambda,Pw,Lw,ew\n";
        for (int i = 0; i < odPairs.size(); i++) {
            auto pw = 1 - std::get<0>(result.at(i));
            auto lw = std::get<1>(result.at(i));
            auto ew = std::get<2>(result.at(i));
            auto od = odPairs.at(i);
            file << i << ','
            << od.getOrigin().first << ','
            << od.getOrigin().second << ','
            << od.getDestination().first << ','
            << od.getDestination().second << ','
            << od.getLambda() << ','
            << pw << ',' << lw << ',' << ew << '\n';
        }
    }
    
    return result;
}
