#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>  // Adicionado para unordered_map
#include <limits>
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
    while (getline(ss, word, ','))
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

pair<double, vector<Vertex<string>*>> dijkstra(Graph<string>& graph, const string &sourceCode, const string &destCode,
                                                 const unordered_set<string>& forbidden) {
    // Usaremos o macro INF definido em graph.h para representar o valor infinito.
    unordered_map<Vertex<string>*, double> dist;
    unordered_map<Vertex<string>*, Vertex<string>*> prev;
    auto vertexSet = graph.getVertexSet();
    for (auto v : vertexSet) {
        dist[v] = INF;
        prev[v] = nullptr;
    }
    // Encontrar vértice de origem e destino (a procura é pelo campo code)
    Vertex<string>* source = nullptr;
    Vertex<string>* dest = nullptr;
    for (auto v : vertexSet) {
        if (v->getCode() == sourceCode)
            source = v;
        if (v->getCode() == destCode)
            dest = v;
    }
    if (!source || !dest) return {INF, {}};

    dist[source] = 0;

    // Fila de prioridade: (distância, vértice)
    auto cmp = [](pair<double, Vertex<string>*> left, pair<double, Vertex<string>*> right) {
        return left.first > right.first;
    };
    priority_queue<pair<double, Vertex<string>*>, vector<pair<double, Vertex<string>*>>, decltype(cmp)> pq(cmp);
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;

        if (u == dest)
            break;

        // Para cada aresta de 'u', considerar apenas a primeira aresta para cada destino (assumindo driving)
        unordered_set<Vertex<string>*> considered;
        for (auto edge : u->getAdj()) {
            Vertex<string>* v = edge->getDest();
            if (considered.find(v) != considered.end())
                continue; // já processou uma aresta para esse destino (suposta de driving)
            considered.insert(v);
            // Se 'v' estiver na lista de proibidos (mas permite se for destino)
            if (forbidden.find(v->getCode()) != forbidden.end() && v->getCode() != destCode)
                continue;
            double alt = dist[u] + edge->getWeight();
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push({alt, v});
            }
        }
    }

    // Reconstruir o caminho
    vector<Vertex<string>*> path;
    if (dist[dest] == INF) return {INF, path};
    for (Vertex<string>* at = dest; at != nullptr; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return {dist[dest], path};
}

void independent_route_planning(Graph<string> &graph) {
    // Lê o input de "input.txt"
    ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        cout << "Error opening input.txt" << endl;
        return;
    }
    string line, sourceStr, destStr;
    getline(inFile, line);
    if (line.find("Source:") != string::npos)
        sourceStr = line.substr(line.find(":") + 1);
    getline(inFile, line);
    if (line.find("Destination:") != string::npos)
        destStr = line.substr(line.find(":") + 1);
    inFile.close();

    // Remover espaços em branco (se existirem)
    sourceStr.erase(remove(sourceStr.begin(), sourceStr.end(), ' '), sourceStr.end());
    destStr.erase(remove(destStr.begin(), destStr.end(), ' '), destStr.end());

    // Calcular rota primária (melhor rota)
    unordered_set<string> emptyForbidden;
    auto [bestTime, bestPath] = dijkstra(graph, sourceStr, destStr, emptyForbidden);

    // Preparar rota alternativa:
    // Proíbe os nós intermediários da rota primária (exceto a origem e destino)
    unordered_set<string> forbidden;
    if (!bestPath.empty()) {
        for (size_t i = 1; i + 1 < bestPath.size(); i++) {
            forbidden.insert(bestPath[i]->getCode());
        }
    }
    auto [altTime, altPath] = dijkstra(graph, sourceStr, destStr, forbidden);

    // Prepara as strings de output de acordo com o formato especificado
    ostringstream bestRouteOSS, altRouteOSS;
    if (bestPath.empty() || bestTime == INF) {
        bestRouteOSS << "BestDrivingRoute:none";
    } else {
        bestRouteOSS << "BestDrivingRoute:";
        for (size_t i = 0; i < bestPath.size(); i++) {
            bestRouteOSS << bestPath[i]->getCode();
            if (i < bestPath.size() - 1)
                bestRouteOSS << ",";
        }
        bestRouteOSS << "(" << bestTime << ")";
    }

    if (altPath.empty() || altTime == INF) {
        altRouteOSS << "AlternativeDrivingRoute:none";
    } else {
        altRouteOSS << "AlternativeDrivingRoute:";
        for (size_t i = 0; i < altPath.size(); i++) {
            altRouteOSS << altPath[i]->getCode();
            if (i < altPath.size() - 1)
                altRouteOSS << ",";
        }
        altRouteOSS << "(" << altTime << ")";
    }

    // Escreve o resultado em "output.txt"
    ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        cout << "Error opening output.txt" << endl;
        return;
    }
    outFile << bestRouteOSS.str() << "\n" << altRouteOSS.str() << "\n";
    outFile.close();

    // Também mostra no terminal os resultados
    cout << bestRouteOSS.str() << "\n" << altRouteOSS.str() << "\n";
}

int main(){
    Graph<string> graph;
    readLocations("locations1.csv", graph);
    readDistances("distances1.csv", graph);

    display();
    int number;
    cin >> number;
    switch (number) {
        case 1:
            // Exibe os vértices e suas adjacências (código original do colega)
            for (const auto &vertex : graph.getVertexSet()) {
                cout << vertex->getInfo() << "\n";

                cout << "Adjacent vertices:\n";

                for (const auto &edge: vertex->getAdj()) {
                    cout << " -> " << edge->getDest()->getLoc()<< " (weight: " << edge->getWeight() << ")\n";
                }
                cout << " --------------";
                cout << "\n";
            }
            // Chamada da funcionalidade de independent route planning em modo batch (input.txt/output.txt)
            independent_route_planning(graph);
            break;

        // Cases para as demais funcionalidades (se implementadas)
    }

    return 0;
}





