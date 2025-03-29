#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "graph.h"
#include "dijkstra.cpp"


using namespace std;


class Algorithm { 
    public:
        vector<string> getNumbers(string line);

        template <class T>
        void independent_route_planningBatchMode(Graph<T> graph);

        template<class T>
        void independent_route_planningInteractiveMode(Graph<T> graph);

        template <class T>
        void restricted_route_planningBatchMode(Graph<T> graph);

        template <class T>
        void ecoFriendlyRoutePlanning(Graph<T> graph);
};
#endif // ALGORITHM_H





