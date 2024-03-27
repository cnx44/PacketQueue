//
// Created by Oliver Quin on 26/03/24.
//

#ifndef PACKETQUEUE_PACKETQUEUECLASSES_H
#define PACKETQUEUE_PACKETQUEUECLASSES_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

struct Station{
    int stationId;
};

class Packet{
public:
    Packet(Station destination, Station currentStation, int size): destination(destination), currentStation(currentStation), size(size){};

private:
    Station destination;
    Station currentStation;
    int size;
};

class Link{
public:
    Link(Station stationA, Station stationB, float rate, float latency):
            stationA(stationA), stationB(stationB), rate(rate), latency(latency){};

private:
    Station stationA;
    Station stationB;
    float rate;
    float latency;
    std::vector<Packet> queue;

};

class Network{
public:
    void NetworkInizialization(Station stationA, Station stationB, float rate, float latency){      //METHOD: It initializes the network with 2 station and the link connecting them
        stationsCardinality = 2;
        Link firstLink(stationA, stationB, rate, latency);
        stations.push_back(stationA);
        stations.push_back(stationB);
        links.push_back(firstLink);


        //Creation of the start adjacency matrix where, as a network should be reciprocal and antisymmetric, the diagonal is set by default to 0 and the secondary diagonal is set to 1
        std::vector<std::vector<int>> tempAdjacencyMatrix(2, std::vector<int>(2, 0));
        tempAdjacencyMatrix[0][1] = 1;
        tempAdjacencyMatrix[1][0] = 1;
        adjacencyMatrix = tempAdjacencyMatrix;      //std::vector should automatically take care of this possible memory leak when out-of-scope
    };

    void InsertStation(Station newStation,Station existingStation, float rate, float latency){
        stationsCardinality++;
        Link newLink(newStation, existingStation, rate, latency);
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
        adjacencyMatrix[newStation.stationId][existingStation.stationId]=1;
        adjacencyMatrix[existingStation.stationId][newStation.stationId]=1;


    };

    void adjacencyMatrixPrint(){
        for (const std::vector<int>& row : adjacencyMatrix) {
            for (int value : row) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    };

private:
    int stationsCardinality;
    std::vector<Station> stations;
    std::vector<Link> links;
    std::vector<std::vector<int>> adjacencyMatrix;
};



#endif //PACKETQUEUE_PACKETQUEUECLASSES_H
