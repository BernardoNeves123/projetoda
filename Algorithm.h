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

/**
 * @class Algorithm
 * @brief A classe contém todos os diferentes métodos de planeamento de rotas(independent, restricted and environmentally-friendly).
 */
class Algorithm { 
    public:

        /**
         * * @brief Função que ao ler uma string, separa os números por vírgula e guarda-os dentro de um vetor.
         * * @param line A string que contém os números separados por vírgula.
         * * @return vector<string>: devolve um vetor com os números separados por vírgula.
         */
        vector<string> getNumbers(string line);

        /**
         * * @brief Função que lê o input.txt e produz o independent route planning, escreve o output.txt com os resultados.
         * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
         * * @return void: escreve dentro do output.txt, então não dá return a nada.
         */
        template <class T>
        void independent_route_planningBatchMode(Graph<T> graph);

        /**
         * * @brief Função que lê o input do utilizador para a origem, o destino, os vértices a ignorar, as arestas a ignorar e o vértice a incluir, e produz o independent route planning, dando print no terminal.
         * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
         * * @return void: dá prints no terminal dos resultados, então não dá return a nada.
         */
        template<class T>
        void independent_route_planningInteractiveMode(Graph<T> graph);

        /**
         * * @brief Função que lê o input.txt e produz o restricted route planning, escreve o output.txt com os resultados.
         * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
         * * @return void: escreve dentro do output.txt, então não dá return a nada.
         */
        template <class T>
        void restricted_route_planningBatchMode(Graph<T> graph);

        /**
         * * @brief Função que lê o input do utilizador para a origem, o destino, os vértices a ignorar, as arestas a ignorar e o vértice a incluir, e produz o restricted route planning, dando print no terminal.
         * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
         * * @return void: dá prints no terminal dos resultados, então não dá return a nada.
         */
        template<class T>
        void restricted_route_planningInteractiveMode(Graph<T> graph);

        /**
         * * @brief Função que lê o input.txt e produz o ecoFriendly route planning, escreve o output.txt com os resultados.
         * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
         * * @return void: escreve dentro do output.txt, então não dá return a nada.
         */
        template <class T>
        void ecoFriendlyRoutePlanningBatchMode(Graph<T> graph);

        /**
         * * @brief Função que lê o input do utilizador para a origem, o destino, o tempo máximo a andar, os vértices para ignorar e as arestas a ignorar, e produz o ecoFriendly route planning, dando print no terminal.
         * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
         * * @return void: dá prints no terminal dos resultados, então não dá return a nada.
         */
        template<class T>
        void ecoFriendlyRoutePlanningInteractiveMode(Graph<T> graph);
};
#endif // ALGORITHM_H





