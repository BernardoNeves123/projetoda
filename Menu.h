
#ifndef MENU_H
#define MENU_H
#include <iostream>
#include "Algorithm.cpp"
#include "Parse.h"
#include "graph.h"
using namespace std;


/**
 * @class Menu
 * @brief A classe oferece ao utilizador uma interface para poder escolher o tipo de route e se é em BatchMode ou através de inputs.
 */
class Menu {
public:
    /**
     * * @brief Função que fornce a interface para escolher o plano de rota, que pode ser através de inputs do utilizador ou através do input.txt/output.txt.
     */
    void display(); 
};

#endif // MENU_H
/**
 * * @brief Função que fornece a interface para escolher o plano de rota, que pode ser através de inputs do utilizador ou através do input.txt/output.txt.
 * * Os diferentes tipos de rotas são: independent, restricted e environmentally-friendly.
 */
void Menu::display() {
    cout << "-----------------------------\n";
    cout << "Select your route planning\n";
    cout << "-----------------------------\n";
    cout << "1: Independent Route Planning\n";
    cout << "2: Restricted Route Planning\n";
    cout << "3: Environmentally-Friendly Route Planning (driving and walking)\n";
    cout << "Enter your choice: ";

    Graph<string> graph;
    
    Parse parse;
    
    parse.readLocations("CSVfiles/Locations.csv",graph);
    parse.readDistances("CSVfiles/Distances.csv", graph);

    
    Algorithm algorithm;
    

    int number;
    cin >> number;
    switch (number) {
        case 1:
 
            int number2;
            cout << "-----------------------------\n";
            cout << "Enter 1 for batch mode or 2 for interactive mode:\n ";
            cout << "-----------------------------\n";
            cin >> number2;
            if (number2 == 1) algorithm.independent_route_planningBatchMode(graph);
            
            else if (number2 == 2) algorithm.independent_route_planningInteractiveMode(graph);
            
            else cout << "Invalid choice. Please try again." << endl;
            
            break;
            
        case 2:
            int number3;
            cout << "-----------------------------\n";
            cout << "Enter 1 for batch mode or 2 for interactive mode:\n ";
            cout << "-----------------------------\n";
            cin >> number3;          
            if (number3 == 1) algorithm.restricted_route_planningBatchMode(graph);
            
            else if (number3 == 2) algorithm.restricted_route_planningInteractiveMode(graph);
            
            else cout << "Invalid choice. Please try again." << endl;
            
            break;

        case 3:
            int number4;
            cout << "-----------------------------\n";
            cout << "Enter 1 for batch mode or 2 for interactive mode:\n ";
            cout << "-----------------------------\n";
            cin >> number4;  
            if (number4 == 1) algorithm.ecoFriendlyRoutePlanningBatchMode(graph);
            
            else if (number4 == 2) algorithm.ecoFriendlyRoutePlanningInteractiveMode(graph);
        
            else cout << "Invalid choice. Please try again." << endl;
        
            break;
;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;

    }
}