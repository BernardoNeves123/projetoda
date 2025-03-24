#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include "graph.h"
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

void readLocations(const char *fileName, Graph<string> &graph) {
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


void readDistances(const char* fileName, Graph<string> &graph) {

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
        graph.addEdge(location1, location2, walking);

    }

    file.close();
}

void calculateIndependentRoutes(Graph<string> &graph) {
    string start, end;
    cout << "Enter source node: ";
    cin >> start;
    cout << "Enter destination node: ";
    cin >> end;

    // Calcular rota principal
    PathResult<string> primary = graph.shortestPathDriving(start, end);

    if (primary.path.empty()) {
        cout << "BestDrivingRoute:none\nAlternativeDrivingRoute:none\n";
        return;
    }

    // Coletar nós e arestas excluídos para a rota alternativa
    unordered_set<string> excludedNodes;
    for (size_t i = 1; i < primary.path.size() - 1; ++i) {
        excludedNodes.insert(primary.path[i]);
    }

    unordered_set<pair<string, string>> excludedEdges;
    for (size_t i = 0; i < primary.path.size() - 1; ++i) {
        string u = primary.path[i];
        string v = primary.path[i + 1];
        excludedEdges.insert({u, v});
        excludedEdges.insert({v, u}); // Para grafos não direcionados
    }

    // Calcular rota alternativa
    PathResult<string> alternative = graph.shortestPathDriving(start, end, excludedNodes, excludedEdges);

    // Exibir resultados
    cout << "BestDrivingRoute:";
    for (size_t i = 0; i < primary.path.size(); ++i) {
        if (i > 0) cout << ",";
        cout << primary.path[i];
    }
    cout << "(" << primary.totalTime << ")\n";

    if (alternative.path.empty() || alternative.totalTime < primary.totalTime) {
        cout << "AlternativeDrivingRoute:none\n";
    } else {
        cout << "AlternativeDrivingRoute:";
        for (size_t i = 0; i < alternative.path.size(); ++i) {
            if (i > 0) cout << ",";
            cout << alternative.path[i];
        }
        cout << "(" << alternative.totalTime << ")\n";
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




    }

    return 0;
}

