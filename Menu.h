
#ifndef MENU_H
#define MENU_H
#include <iostream>
#include "Algorithm.cpp"
#include "Parse.h"
#include "graph.h"
using namespace std;

class Menu {
public:
    
    void display(); // Declaration of the display() function
};

#endif // MENU_H

void Menu::display() {
    cout << "Select your route planning\n";
    cout << "-----------------------------\n";
    cout << "1: Independent Route Planning\n";
    cout << "2: Restricted Route Planning\n";
    cout << "3: Environmentally-Friendly Route Planning (driving and walking)\n";
    cout << "Enter your choice: ";

    Graph<string> graph;
    
    Parse parse;
    
    parse.readLocations("CSVfiles/Loc.csv",graph);
    parse.readDistances("CSVfiles/Dis.csv", graph);

    
    Algorithm algorithm;
    

    int number;
    cin >> number;
    switch (number) {
        case 1:
            int number2;
            cout << "Enter 1 for batch mode or 2 for interactive mode: ";
            cin >> number2;
            if (number2 == 1) {
                algorithm.independent_route_planningBatchMode(graph);
            } else if (number2 == 2) {
                algorithm.independent_route_planningInteractiveMode(graph);
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
            break;
            
        case 2:
            algorithm.restricted_route_planningBatchMode(graph);
            break;
        case 3:
            algorithm.ecoFriendlyRoutePlanning(graph);
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;

    }
}