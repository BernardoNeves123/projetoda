#include "Algorithm.h"

vector<string> Algorithm::getNumbers(string line){
    vector<string> numbers;
    stringstream ss(line);
    string word;
    while (getline(ss,word, ','))
        numbers.push_back(word);

    return numbers;
}

template<class T>
void Algorithm::independent_route_planningBatchMode(Graph<T> graph) {
    
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
    MyFile.close();
}

template<class T>
void Algorithm::independent_route_planningInteractiveMode(Graph<T> graph){
    cout << "Enter the source: ";
    T source;
    cin >> source;
    
    cout << "Enter the destination: ";
    T destination;
    cin >> destination;

    int sourceIDx = graph.findVertexIdx(source);
    Vertex<T> *sourceVertex1 = graph.getVertexSet()[sourceIDx];
    string code1 = sourceVertex1->getCode();

    dijkstra(&graph, code1);

    int destIDx = graph.findVertexIdx(destination);
    Vertex<T> *destVertex1 = graph.getVertexSet()[destIDx];
    string code2 = destVertex1->getCode();

    vector<T> result = getPath(&graph, code1, code2);

    cout << "BestDrivingRoute:";
    int totalDistance = 0;

    for (size_t i = 0; i < result.size(); i++) {


        int idx = graph.findVertexIdxCode(result[i]);
        Vertex<T> *vertex = graph.getVertexSet()[idx];

        cout << vertex->getId();
        if (i < result.size() - 1) {
            cout << ",";
        }
    }
    for (auto vertex : graph.getVertexSet()) {
        if (vertex->getCode() == code2) {
            totalDistance = vertex->getDist();
        }
    }

    cout << "(" << totalDistance << ")" << endl;

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
    cout << "AlternativeDrivingRoute:";
    int totalDistance2 = 0;

    if (result2.size() == 0) {
        cout << "none" << endl;
    }
    else {
        for (size_t i = 0; i < result2.size(); i++) {
            int idx = graph.findVertexIdxCode(result2[i]);
            Vertex<T> *vertex = graph.getVertexSet()[idx];

            cout << vertex->getId();  // Write vertex ID
            if (i < result2.size() - 1) {
                cout << ",";
            }
        }
        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code2) {
                totalDistance2 = vertex->getDist();
            }
        }

        cout << "(" << totalDistance2 << ")";
    }

}

template<class T>
void Algorithm::restricted_route_planningBatchMode(Graph<T> graph) {
    
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
            stringstream ss(code);

            while (getline(ss, code, ')')) {
                size_t open = code.find('(');

                size_t comma = code.find(',',  open +1);


                T source1 = code.substr(open + 1, comma - open - 1);
                int sourceIDx = graph.findVertexIdx(source1);
                Vertex<T> *sourceVertex2 = graph.getVertexSet()[sourceIDx];
                string code4 = sourceVertex2->getCode();

                T destination1 = code.substr(comma + 1);
                int destIDx = graph.findVertexIdx(destination1);
                Vertex<T> *destinationVertex2 = graph.getVertexSet()[destIDx];
                string code5 = destinationVertex2->getCode();

                if (auto e = graph.findEdge(code4, code5)) e->setIgnore(true);
            }

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
    MyFile.close();
}

