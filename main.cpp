#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include "graph.h"
#include "dijkstra.cpp"
using namespace std;

void display() {
    cout << "Select your route planning\n";
    cout << "-----------------------------\n";
    cout << "1: Independent Route Planning\n";
    cout << "2: Restricted Route Planning\n";
    cout << "3: Environmentally-Friendly Route Planning (driving and walking)\n";
}

vector<string> getNumbers(string line) {
    vector<string> numbers;
    stringstream ss(line);
    string word;
    while (getline(ss,word, ','))
        numbers.push_back(word);

    return numbers;
}

template <class T>
void readLocations(const char *fileName, Graph<T> &graph) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error opening file " << fileName << endl;
        exit(1);
    }

    string line;

    getline(file, line);

    while (getline(file, line)) {
        vector<string> numbers = getNumbers(line);

        T location = numbers[0];
        T id = (numbers[1]);

        string code = numbers[2];

        int parking = stoi(numbers[3]);

        graph.addVertex(location, id, code, parking);
    }

    file.close();
}

template <class T>
void readDistances(const char* fileName, Graph<T> &graph) {

    ifstream file(fileName);
    vector<string> numbers;

    if (!file.is_open()) {
        cout << "Error opening file " << fileName << "\n";
    }

    string line;

    getline(file, line);

    while (getline(file, line)) {
        numbers = getNumbers(line);

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

template<class T>
void independent_route_planning(Graph<T> graph) {
    const char *fileName = "input.txt";
    ifstream inputFile(fileName);
    vector<string> numbers;
    if (!inputFile.is_open()) {
        cout << "Error opening file " << fileName << endl;
    }
    string line;
    getline(inputFile, line);
    while (getline(inputFile, line)) {
        size_t division = line.find(":");
        string code = line.substr(division + 1);


        numbers.push_back(code);
    }
    T source = (numbers[0]);
    int sourceIDx = graph.findVertexIdx(source);
    Vertex<T> *sourceVertex1 = graph.getVertexSet()[sourceIDx];
    string code1 = sourceVertex1->getCode();

    dijkstra(&graph, code1);

    T destination = (numbers[1]);
    int destIDx = graph.findVertexIdx(destination);
    Vertex<T> *destVertex1 = graph.getVertexSet()[destIDx];
    string code2 = destVertex1->getCode();

    vector<T> result = getPath(&graph, code1, code2);

    ofstream MyFile("output.txt");

    MyFile << "Source:" << source << endl;
    MyFile << "Destination:" << destination << endl;


    MyFile << "BestDrivingRoute:";
    int totalDistance = 0;

    for (size_t i = 0; i < result.size(); i++) {


        int idx = graph.findVertexIdxCode(result[i]);
        Vertex<T> *vertex = graph.getVertexSet()[idx];

        MyFile << vertex->getId();
        if (i < result.size() - 1) {
            MyFile << ",";
        }
    }
    for (auto vertex : graph.getVertexSet()) {
        if (vertex->getCode() == code2) {
            totalDistance = vertex->getDist();
        }
    }

    MyFile << "(" << totalDistance << ")" << endl;

    vector<T> verticesToRemove;
    for (size_t i = 1; i < result.size() - 1; i++) {

        verticesToRemove.push_back(result[i]);
    }

    for (const T &vertexCode : verticesToRemove) {
        auto v = graph.findVertex(vertexCode);
        v->setIgnore(true);
    }


    dijkstra(&graph, code1);
    vector<T> result2 = getPath(&graph, code1, code2);
    MyFile << "AlternativeDrivingRoute:";
    int totalDistance2 = 0;

    if (result2.size() == 0) {
        MyFile << "none" << endl;
    }
    else {
        for (size_t i = 0; i < result2.size(); i++) {
            int idx = graph.findVertexIdxCode(result2[i]);
            Vertex<T> *vertex = graph.getVertexSet()[idx];

            MyFile << vertex->getId();  // Write vertex ID
            if (i < result2.size() - 1) {
                MyFile << ",";
            }
        }
        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code2) {
                totalDistance2 = vertex->getDist();
            }
        }

        MyFile << "(" << totalDistance2 << ")";
    }

}

template<class T>
void restricted_route_planning(Graph<T> graph) {
    const char *fileName = "input.txt";
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error opening file " << fileName << endl;
    }

    T source, destination;
    vector<T> avoidNodes;
    vector<pair<T, T>> avoidSegments;
    string includeNode;

    string line;
    getline(inputFile, line);
    while (getline(inputFile, line)) {
        size_t division = line.find(":");
        string location = line.substr(0,division);
        string code = line.substr(division + 1);

        if (location == "Source") source = (code);
        else if (location == "Destination") destination = (code);
        else if (location == "AvoidNodes") {
            avoidNodes = getNumbers(code);
        }
        else if (location == "AvoidSegments") {

        }
        else if (location == "IncludeNode") includeNode = (code);

    }

    int sourceIDx = graph.findVertexIdx(source);
    Vertex<T> *sourceVertex1 = graph.getVertexSet()[sourceIDx]; //Vertex of the source
    string code1 = sourceVertex1->getCode();

    for (const T &vertexCode : avoidNodes) {
        auto v = graph.findVertex(vertexCode);
        v->setIgnore(true); //nodes to not visit
    }



    int destIDx = graph.findVertexIdx(destination);
    Vertex<T> *destVertex1 = graph.getVertexSet()[destIDx]; //Vertex for the destination
    string code2 = destVertex1->getCode();



    vector<T> result;
    int totalDistanceToNode = 0;
    int totalDistanceToDest = 0;
    int totalDistance = 0;

    if (!includeNode.empty()) {

        int nodeIDx = graph.findVertexIdx(includeNode);
        Vertex<T> *nodeVertex = graph.getVertexSet()[nodeIDx];
        string code3 = nodeVertex->getCode();

        dijkstra(&graph, code1);
        vector<T> pathtoNode = getPath(&graph, code1, code3);

        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code3) {
                totalDistanceToNode = vertex->getDist();
            }
        }

        dijkstra(&graph, code3);
        vector<T> pathNodetoDestination = getPath(&graph, code3, code2);

        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code2) {
                totalDistanceToDest = vertex->getDist();
            }
        }


        result.insert(result.end(), pathtoNode.begin(), pathtoNode.end());
        result.insert(result.end(), pathNodetoDestination.begin() + 1, pathNodetoDestination.end());



        totalDistance =  totalDistanceToNode + totalDistanceToDest;

    }
    else {
        dijkstra(&graph, code1);
        result = getPath(&graph, code1, code2);

        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code2) {
                totalDistance = vertex->getDist();
            }
        }
    }


    ofstream MyFile("output.txt");

    MyFile << "Source:" << source << endl;
    MyFile << "Destination:" << destination << endl;


    MyFile << "RestrictedDrivingRoute:";

    if (result.size() == 0) {
        MyFile << "none" << endl;
    }
    else {
        for (size_t i = 0; i < result.size(); i++) {
            int idx = graph.findVertexIdxCode(result[i]);
            Vertex<T> *vertex = graph.getVertexSet()[idx];

            MyFile << vertex->getId();  // Write vertex ID
            if (i < result.size() - 1) {
                MyFile << ",";
            }
        }


        MyFile << "(" << totalDistance << ")";
    }
}

int main(){
    Graph<string> graph;
    readLocations("Loc.csv",graph);
    readDistances("Dis.csv", graph);


    display();
    int number;
    cin >> number;
    switch (number) {
        case 1:
            independent_route_planning(graph);
        case 2:
            restricted_route_planning(graph);




    }

    return 0;
}

