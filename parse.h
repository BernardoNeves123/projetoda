#ifndef PARSE_H
#define PARSE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


#include "Algorithm.h"
#include "graph.h"

/**
 * * @class Parse
 * * @brief A classe lê os ficheiros CSV e guarda as localizações e distâncias dentro do grafo.
 * * @details A classe contém dois métodos: readLocations e readDistances.
 */

class Parse {
public:

    /**
     * * @brief Função que lê o ficheiro CSV com as localizações e guarda-as dentro do grafo como vértices.
     * * @param fileName O nome do ficheiro CSV com as localizações.
     * * @param graph O grafo onde vamos guardar as localizações.
     */
    template <class T>
    void readLocations(const char *fileName, Graph<T> &graph);

    /**
     * * @brief Função que lê o ficheiro CSV com as distâncias e guarda-as dentro do grafo como arestas.
     * * @param fileName O nome do ficheiro CSV com as distâncias.
     * * @param graph O grafo onde vamos guardar as distâncias.
     */
    template <class T>
    void readDistances(const char* fileName, Graph<T> &graph);


};
#endif // PARSE_H
/**
     * * @brief Função que lê o ficheiro CSV com as localizações e guarda-as dentro do grafo como vértices.
     * * @param fileName O nome do ficheiro CSV com as localizações.
     * * @param graph O grafo onde vamos guardar as localizações.
     * * @return void: não devolve nada.
     * * @details Location: nome do vértice.
     * *          id: id do vértice.
     * *          Code: código do vértice.
     * *          Parking: se o vértice tem estacionamento ou não.
     */
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
/**
     * * @brief Função que lê o ficheiro CSV com as distâncias e guarda-as dentro do grafo como arestas.
     * * @param fileName O nome do ficheiro CSV com as distâncias.
     * * @param graph O grafo onde vamos guardar as distâncias.
     * * @return void: não devolve nada.
     * * @details Location1: nome do vértice 1.
     * *          Location2: nome do vértice 2.
     * *          Driving: distância entre os dois vértices a conduzir.
     * *          Walking: distância entre os dois vértices a pé.
     */

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
        double driving = INF; //maior valor possível
        if (numbers[2] != "X")  driving = stoi(numbers[2]);

        double walking = stoi(numbers[3]);



        graph.addBidirectionalEdge(location1, location2, driving, "driving");
        graph.addBidirectionalEdge(location1, location2, walking, "walking");

    }

    file.close();
}

