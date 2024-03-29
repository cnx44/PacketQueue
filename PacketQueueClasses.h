//
// Created by Oliver Quin on 26/03/24.
//

#ifndef PACKETQUEUE_PACKETQUEUECLASSES_H
#define PACKETQUEUE_PACKETQUEUECLASSES_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

struct Station{
    int stationId;
};

struct Packet{
    Station endStation;
    Station currentStation;
    int size;
    std::vector<Station> path;
};

struct  Link{
    Station stationA;
    Station stationB;
    float rate;
    float latency;
    std::queue<Packet> queue;

};

class Network{
public:
    void NetworkInizialization(Station stationA, Station stationB, float rate, float latency){      //METHOD: It initializes the network with 2 station and the link connecting them
        stationsCardinality = 2;
        Link firstLink{stationA, stationB, rate, latency};
        stations.push_back(stationA);
        stations.push_back(stationB);
        links.push_back(firstLink);


        //Creation of the start adjacency matrix where, as a network should be reciprocal and antisymmetric, the diagonal is set by default to 0 and the secondary diagonal is set to 1
        std::vector<std::vector<int>> tempAdjacencyMatrix(2, std::vector<int>(2, 0));
        tempAdjacencyMatrix[0][1] = 1;
        tempAdjacencyMatrix[1][0] = 1;
        adjacencyMatrix = tempAdjacencyMatrix;      //std::vector should automatically take care of this possible memory leak when out-of-scope
    };

    void adjacencyMatrixPrint(){    //Temporary method using for convenience in testing phase
        for (const std::vector<int>& row : adjacencyMatrix) {
            for (int value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    };

    //TODO: the current ambiguity that the dichotomy between Station and int, even tho their now basically the same thing, creates an uncomfortably har code to read.
    // A possible fix is to substitute every instance of a dualism of int-Station with a METHOD which return the said station from the id
    //TODO: switching from a BFS to a Dijkstra may be beneficial as the code is intended
    std::vector<Station> PathFinder(Station startStation, Station endStation){                          //METHOD: Implementation of BFS
        std::vector<bool> visitedStation(stationsCardinality, false);          //sizeof(bool)==sizeof(char)<sizeof(int)
        std::queue<Station> stationQueue;
        std::vector<int> parentStationId(stationsCardinality, -1);

        stationQueue.push(startStation);
        visitedStation[startStation.stationId]=true;

        while(!stationQueue.empty()){
            Station currentStation = stationQueue.front();
            stationQueue.pop();
            
            if(currentStation.stationId == endStation.stationId){
                std::vector<Station> path;
                for(int stationId = endStation.stationId; stationId != -1; stationId = parentStationId[stationId]){
                    Station tempStation {stationId};
                    path.push_back(tempStation);
                }

                std::reverse(path.begin(),path.end());
                return path;
            }

            for(int nextStationId = 0; nextStationId<stationsCardinality; ++nextStationId){
                if(adjacencyMatrix[currentStation.stationId][nextStationId] == 1 && !visitedStation[nextStationId]){
                    Station unexploredStation{nextStationId};
                    stationQueue.push(unexploredStation);
                    visitedStation[nextStationId] = true;
                    parentStationId[nextStationId] = currentStation.stationId;
                }
            }
        }

        return {};  //Empty vector, no path found
    };

    void LinkCreator(Station newStation,Station existingStation, float rate, float latency){    //METHOD: define a link between two stations, float rate[Mb/s], float latency [s]
        stationsCardinality++;
        Link newLink{newStation, existingStation, rate, latency};
        stations.push_back(newStation);
        links.push_back(newLink);

        //Adaptation fo adjacency matrix on network update
        //Creating new row with default value 0
        adjacencyMatrix.resize(adjacencyMatrix.size()+1,std::vector(adjacencyMatrix[0].size(),0));
        //Creating new column with default value 0
        for (std::vector<int>& row : adjacencyMatrix) {     //I'm unaware of more efficient way to expand a std::vector "matrix" by 1 column more efficiently
            row.resize(row.size() + 1, 0);
        }

        //As the initialization case the matrix has to be reciprocal and antisymmetric
        //the code here take care for the reciprocal propriety,
        // the antisymmetry has to be taken care on newStation creation to ensure the station inserted wasn't alredy in the list
        adjacencyMatrix[newStation.stationId][existingStation.stationId] = 1;
        adjacencyMatrix[existingStation.stationId][newStation.stationId] = 1;


    };

    void StationCreator(){      //METHOD: used to crete ad insert in std::vector<Station> sations
        Station newStation{stationsCardinality};
        stations.push_back(newStation);
        stationsCardinality++;
    };

    void PacketCreator(int size, Station starterStation, Station endStation){
        Packet newPacket{starterStation, endStation, size, PathFinder(starterStation,endStation)};
        Link chosenLink;
        for (const Link& s : links) {   //Loop used to find on which link the packet should stack
            if(s.stationA.stationId == newPacket.path[0].stationId){    // Check if the n-th station id of the std::vector<Link>
                chosenLink = s;                                         //links the same as the starter station of the packet's path
                chosenLink.queue.push(newPacket);
                links.push_back(chosenLink);
                return;     //equivalent of break when the right link is found, but more in line with Google Cpp StyleGuide
            }
        }
    };

    //TODO: This METHOD should act as the "next step" work in debugging, where as invoked it moves
    // the timeline at the moment where the first packet is received and moved on the next link
    void NextStep(){};

private:
    int stationsCardinality=0;
    std::vector<Station> stations;
    std::vector<Link> links;
    std::vector<std::vector<int>> adjacencyMatrix;
};



#endif //PACKETQUEUE_PACKETQUEUECLASSES_H
