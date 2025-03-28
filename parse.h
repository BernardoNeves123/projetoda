#ifndef PARSE_H
#define PARSE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


#include "Algorithm.h"
#include "graph.h"

class Parse {
public:
    template <class T>
    void readLocations(const char *fileName, Graph<T> &graph);

    template <class T>
    void readDistances(const char* fileName, Graph<T> &graph);

};
#endif // PARSE_H

template <class T>
void Parse::readLocations(const char *fileName, Graph<T> &graph) {
    Algorithm algorithm;
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error opening file " << fileName << endl;
        exit(1);
    }

    string line;

    getline(file, line);

    while (getline(file, line)) {
        vector<string> numbers = algorithm.getNumbers(line);

        T location = numbers[0];
        T id = (numbers[1]);

        string code = numbers[2];

        int parking = stoi(numbers[3]);

        graph.addVertex(location, id, code, parking);
    }

    file.close();
}


template <class T>
void Parse::readDistances(const char* fileName, Graph<T> &graph) {

    Algorithm algorithm;
    ifstream file(fileName);
    vector<string> numbers;

    if (!file.is_open()) {
        cout << "Error opening file " << fileName << "\n";
    }

    string line;

    getline(file, line);

    while (getline(file, line)) {
        numbers = algorithm.getNumbers(line);

        T location1 = numbers[0];
        T location2 = numbers[1];

        if (numbers[2] == "X") continue;
        double driving = stoi(numbers[2]);

        double walking = stoi(numbers[3]);



        graph.addBidirectionalEdge(location1, location2, driving);
        //graph.addEdge(location1, location2, walking);

    }

    file.close();
}