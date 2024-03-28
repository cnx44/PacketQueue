#include <iostream>
#include "PacketQueueClasses.h"


int main(){
    Station station0{0};
    Station station1{1};
    Station station2{2};
    Station station3{3};

    Network mainNetwork;
    mainNetwork.NetworkInizialization(station0,station1,0,0);
    mainNetwork.InsertStation(station2,station1,0,0);
    mainNetwork.InsertStation(station3,station0,0,0);

    mainNetwork.adjacencyMatrixPrint();

    auto s=mainNetwork.PathFinder(station0,station3);

    for(int i=0;i<s.size();i++){
        std::cout<<s[i].stationId<<" ";
    }

    return 0;
}