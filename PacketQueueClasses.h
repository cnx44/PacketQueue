//
// Created by Oliver Quin on 26/03/24.
//

#ifndef PACKETQUEUE_PACKETQUEUECLASSES_H
#define PACKETQUEUE_PACKETQUEUECLASSES_H

#import <iostream>
#import <string>
#import <vector>

struct Station{
    std::string name;
    std::vector<Station> adjacentStation;
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
    Network(std::vector<Station> stations, std::vector<Link> links): stations(stations), links(links){};
    void AdjacencyListCreator(){
        int stationsSize = stations.size();
        int linksSize = links.size();

        //TODO: create function to generate adjacency list for Stations
    };

private:
    int stationsCardinality;
    std::vector<Station> stations;
    std::vector<Link> links;
};



#endif //PACKETQUEUE_PACKETQUEUECLASSES_H
