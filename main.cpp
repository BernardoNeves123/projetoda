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

vector<string> getInputNumbers(string line) {
    vector<string> numbers;
    stringstream ss(line);
    string word;
    while (getline(ss,word, ':'))
        numbers.push_back(word);

    vector<string> result;
    result.push_back(numbers[1]);
    result.push_back(numbers[3]);
    return result;
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

        string location = numbers[0];
        int id = stoi(numbers[1]);

        string code = numbers[2];

        int parking = stoi(numbers[3]);
        //cout << location << " " << id << " " << code << " " << parking << endl;

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

        string location1 = numbers[0];
        string location2 = numbers[1];

        if (numbers[2] == "X") continue;
        double driving = stoi(numbers[2]);

        double walking = stoi(numbers[3]);



        graph.addEdge(location1, location2, driving);
        //graph.addEdge(location1, location2, walking); commented for now because the first half is only driving

    }

    file.close();
}

template <class T>
void restricted_route_planning(Graph<T> &graph) {


    const char *fileName = "../Input/input.txt";
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error opening file " << fileName << endl;
        exit(1);
    }
}

//template <class T>
//void independent_route_planning(Graph<T> &graph) {
    //const char *fileName = "../Input/input.txt";
    //ifstream inputFile(fileName);
    //vector<string> numbers;

    //if (!inputFile.is_open()) {
        //cout << "Error opening file " << fileName << endl;
    //}
    //string line;
    //getline(inputFile, line);
    //while (getline(inputFile, line)) {
       // numbers = getInputNumbers(line);
    //}

     //const int &source = stoi(numbers[0]);
     //const int &destination = stoi(numbers[1]);

    //dijkstra(&graph, source);
    //vector<int> result = getPath(&graph,source,destination);
    //for (const auto &vertex : result) {
        //cout << vertex << ",";
    //}
//}

template<class T>
int run(){
    Graph<T> graph;
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
        //case 2:
            //independent_route_planning(graph);



    }

    return 0;
}

int main() {
     run<string>();
}