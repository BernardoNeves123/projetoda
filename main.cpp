#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
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
        string location = line.substr(0, division);
        string code = line.substr(division + 1);


        numbers.push_back(code);
    }

    T source = (numbers[0]);
    int sourceIDx = graph.findVertexIdx(source);
    Vertex<T> *sourceVertex1 = graph.getVertexSet()[sourceIDx];
    string code1 = sourceVertex1->getCode();

    if (!sourceVertex1) {
        cout << "Error: Source vertex not found in graph." << endl;
        return;
    }


    dijkstra(&graph, code1);
    for (auto vertex : graph.getVertexSet()) {
        cout << "Vertex: " << vertex->getInfo() << ", Distance: " << vertex->getDist() << endl;
    }

    T destination = (numbers[1]);
    int destIDx = graph.findVertexIdx(destination);
    Vertex<T> *destVertex1 = graph.getVertexSet()[destIDx];
    string code2 = destVertex1->getCode();

    vector<T> result = getPath(&graph, code1, code2);

    ofstream MyFile("output.txt");
    MyFile << "Mode:driving";
    
    for (auto vertexCode : result) {
        int a = graph.findVertexIdxCode(vertexCode);
        Vertex<T> *results = graph.getVertexSet()[a];
        cout << results->getId() << ",";
    }

}


int main(){
    Graph<string> graph;
    readLocations("locations1.csv",graph);
    readDistances("distances1.csv", graph);


    display();
    int number;
    cin >> number;
    switch (number) {
        case 1:
            for (const auto &vertex : graph.getVertexSet()) {
                cout << vertex->getInfo() << "\n";

                cout << "Adjacent vertices:\n";

                for (const auto &edge: vertex->getAdj()) {
                    cout << " -> " << edge->getDest()->getLoc()<< " (weight: " << edge->getWeight() << ")\n";
                }
                cout << " --------------";
                cout << "\n";
            }
        case 2:
            independent_route_planning(graph);




    }

    return 0;
}

