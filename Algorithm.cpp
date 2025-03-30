#include "Algorithm.h"

vector<string> Algorithm::getNumbers(string line){ //função para separar os números das virgulas, e pô-los dentro de um vetor
    vector<string> numbers; //vector para guardar os números
    stringstream ss(line); //cria um stringstream para separar os números
    string word; //número que queremos guardar em forma de string
    while (getline(ss,word, ',')) //vamos linha a linha, separando os números por vírgula
        numbers.push_back(word); //guardamos os números dentro do vetor

    return numbers; //return do vetor
}

/**
 * * @brief Função que lê o input.txt e produz o independent route planning, escreve o output.txt com os resultados.
 * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
 * * @return void: escreve dentro do output.txt, então não dá return a nada.
 * * @details A função lê o input.txt, que contém a origem e o destino em strings, armazena-os dentro de um vetor.
 * * Com o primeiro elemento do vetor, temos a origem, encontramos o vértice, e com ele fazemos o dijkstra de modo a sabermos os caminhos mais rápidos para todos os outros vértices.
 * * Depois, com o segundo elemento do vetor, encontramos o vértice que pretendemos acabar.
 * * Chamamos a função getPath, que nos dá o caminho mais curto entre a origem e o destino dentro de um vetor.
 * * Dentro do ficheiro output.txt, escrevemos a origem, o destino e o caminho mais curto entre os dois(com os ids), com a distância total.
 * * Depois, percorremos o vetor de forma a encontrar os vértices que estão entre a origem e o destino, e marcamos esses vértices como "ignore" para não serem visitados na próxima vez que chamarmos o dijkstra.
 * * Com o mesmo processo temos o caminho alternativo.
 */
template<class T>
void Algorithm::independent_route_planningBatchMode(Graph<T> graph) { //função que lê o input.txt e faz o independent route planning
    
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

    dijkstra(&graph, code1, "driving");

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


    dijkstra(&graph, code1, "driving");
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


/**
 * * @brief Função que lê o input do utilizador para a origem e para o destino, e produz o independent route planning, dando print no terminal do BestDrivingRoute e AlternativeDrivingRoute.
 * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
 * * @return void: dá prints no terminal dos resultados, então não dá return a nada.
 * * @details A função lê os inputs da origem e do destino em strings.
 * * Temos a origem, encontramos o vértice, e com ele fazemos o dijkstra de modo a sabermos os caminhos mais rápidos para todos os outros vértices.
 * * Chamamos a função getPath, que nos dá o caminho mais curto entre a origem e o destino dentro de um vetor.
 * * No terminal, damos print à origem, ao destino e ao caminho mais curto entre os dois(com os ids), com a distância total(BestDrivingRoute).
 * * Depois, percorremos o vetor de forma a encontrar os vértices que estão entre a origem e o destino, e marcamos esses vértices como "ignore" para não serem visitados na próxima vez que chamarmos o dijkstra.
 * * Com o mesmo processo temos o caminho alternativo dando print com a sua distância total(AlternativeDrivingRoute).
 */
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

    dijkstra(&graph, code1, "driving");

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


    dijkstra(&graph, code1, "driving");
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

/**
 * * @brief Função que lê o input.txt e produz o restricted route planning, escreve o output.txt com os resultados.
 * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
 * * @return void: escreve dentro do output.txt, então não dá return a nada.
 * * @details A função lê o input.txt, que contém a origem, o destino, o vértice para ignorar, as arestas a ignorar e o vértice que se tem de incluir em strings.
 * * Com o primeiro elemento do vetor, temos a origem, encontramos o vértice, e com ele fazemos o dijkstra de modo a sabermos os caminhos mais rápidos para todos os outros vértices.
 * * Depois, com o segundo elemento do vetor, encontramos o vértice que pretendemos acabar.
 * * Chamamos a função getPath, que nos dá o caminho mais curto entre a origem e o destino dentro de um vetor.
 * * Dentro do ficheiro output.txt, escrevemos a origem, o destino e o caminho mais curto entre os dois(com os ids), com a distância total.
 * * Depois, percorremos o vetor de forma a encontrar os vértices que estão entre a origem e o destino, e marcamos esses vértices como "ignore" para não serem visitados na próxima vez que chamarmos o dijkstra.
 * * Com o mesmo processo temos o caminho alternativo.
 */

template<class T>
void Algorithm::restricted_route_planningBatchMode(Graph<T> graph) {
    
    const char *fileName = "input.txt";
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error opening file " << fileName << endl;
    }

    T source, destination;
    vector<T> avoidNodes;
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

        dijkstra(&graph, code1, "driving");
        vector<T> pathtoNode = getPath(&graph, code1, code3);

        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code3) {
                totalDistanceToNode = vertex->getDist();
            }
        }

        dijkstra(&graph, code3, "driving");
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
        dijkstra(&graph, code1, "driving");
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

/**
 * * @brief Função que lê o input do utilizador para a origem, o destino, os vértices a ignorar, as arestas a ignorar e o vértice a incluir, e produz o restricted route planning, dando print no terminal do RestrictedDrivingRoute.
 * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
 * * @return void: dá prints no terminal dos resultados, então não dá return a nada.
 * * @details A função lê os inputs da origem, do destino, dos vértices a ignorar, das arestas a ignorar e do vértice a incluir em strings.
 * * Com a função getNumbers, obtemos os números que estão separados por vírgula e guardamos dentro de um vetor, marcamos no grafo os vértices para não passar.
 * * Através do input do avoidSegments, obtemos as arestas a ignorar e marcamos no grafo as arestas para não passar.
 * * Depois, com o input do includeNode, obtemos o vértice a incluir.
 * * Se houver includeNode, fazemos o dijkstra da origem ao includeNode guardando os valores num vetor. A variável totalDistanceToNode guarda a distância total do caminho da origem ao includeNode.
 * * Depois, fazemos o dijkstra do includeNode ao destino, guardando os valores apartir do segundo vértice no mesmo vetor. A variável totalDistanceToDest guarda a distância total do caminho do includeNode ao destino.
 * * Com o totalDistanceToNode e o totalDistanceToDest, obtemos a distância total do caminho da origem ao destino.
 * * Se não houver includeNode, fazemos o dijkstra da origem ao destino, guardando os valores num vetor. A variável totalDistance guarda a distância total do caminho da origem ao destino.
 */

template<class T>
void Algorithm::restricted_route_planningInteractiveMode(Graph<T> graph){
    cout << "Enter the source: ";
    T source;
    cin >> source;

    int sourceIDx = graph.findVertexIdx(source);
    Vertex<T> *sourceVertex1 = graph.getVertexSet()[sourceIDx]; //Vertex of the source
    string code1 = sourceVertex1->getCode();
    
    cout << "Enter the destination: ";
    T destination;
    cin >> destination;

    int destIDx = graph.findVertexIdx(destination);
    Vertex<T> *destVertex1 = graph.getVertexSet()[destIDx]; //Vertex for the destination
    string code2 = destVertex1->getCode();

    cout << "Enter the nodes to avoid: ";
    string avoidNodesInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, avoidNodesInput); // Read the avoid nodes input
    
    vector<T> avoidNodes = getNumbers(avoidNodesInput);

    for (const T &vertexCode : avoidNodes) {
        auto v = graph.findVertex(vertexCode);
        v->setIgnore(true); //nodes to not visit
    }

    cout << "Enter the segments to avoid: ";
    string avoidSegmentsInput;
    getline(cin, avoidSegmentsInput); // Read the avoid segments input
    
    
    stringstream ss(avoidSegmentsInput);

    while (getline(ss, avoidSegmentsInput, ')')) {
        size_t open = avoidSegmentsInput.find('(');

        size_t comma = avoidSegmentsInput.find(',',  open +1);


        T source1 = avoidSegmentsInput.substr(open + 1, comma - open - 1);
        int sourceIDx = graph.findVertexIdx(source1);
        Vertex<T> *sourceVertex2 = graph.getVertexSet()[sourceIDx];
        string code4 = sourceVertex2->getCode();

        T destination1 = avoidSegmentsInput.substr(comma + 1);
        int destIDx = graph.findVertexIdx(destination1);
        Vertex<T> *destinationVertex2 = graph.getVertexSet()[destIDx];
        string code5 = destinationVertex2->getCode();

        if (auto e = graph.findEdge(code4, code5)) e->setIgnore(true);
    }

    cout << "Enter the node to include: ";
    string includeNode;
    getline(cin, includeNode); // Read the include node
    

    vector<T> result;
    int totalDistanceToNode = 0;
    int totalDistanceToDest = 0;
    int totalDistance = 0;

    if (!includeNode.empty()) {

        int nodeIDx = graph.findVertexIdx(includeNode);
        Vertex<T> *nodeVertex = graph.getVertexSet()[nodeIDx];
        string code3 = nodeVertex->getCode();

        dijkstra(&graph, code1, "driving");
        vector<T> pathtoNode = getPath(&graph, code1, code3);

        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code3) {
                totalDistanceToNode = vertex->getDist();
            }
        }

        dijkstra(&graph, code3, "driving");
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
        dijkstra(&graph, code1, "driving");
        result = getPath(&graph, code1, code2);

        for (auto vertex : graph.getVertexSet()) {
            if (vertex->getCode() == code2) {
                totalDistance = vertex->getDist();
            }
        }
    }

    cout << "Source:" << source << endl;
    cout << "Destination:" << destination << endl;
    cout << "RestrictedDrivingRoute:";

    if (result.size() == 0) {
        cout << "none" << endl;
    }
    else {
        for (size_t i = 0; i < result.size(); i++) {
            int idx = graph.findVertexIdxCode(result[i]);
            Vertex<T> *vertex = graph.getVertexSet()[idx];

            cout << vertex->getId();  // Write vertex ID
            if (i < result.size() - 1) {
                cout << ",";
            }
        }

        cout << "(" << totalDistance << ")";
    }
}

/**
 * * @brief Função que lê o input.txt e produz o restricted route planning, escreve o output.txt com os resultados.
 * * @param graph O grafo onde vamos trabalhar, que produz as localizações com as distâncias.
 * * @return void: escreve dentro do output.txt, então não dá return a nada.
 * * @details A função lê o input.txt, que contém a origem, o destino, o tempo máximo a andar, os vértices para ignorar e as arestas a ignorar.
 * 
 */

template <class T>
void Algorithm::ecoFriendlyRoutePlanning(Graph<T> graph) {
    const char *fileName = "input.txt";
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error opening file " << fileName << endl;
        return;
    }

    // Variáveis para armazenar os dados do arquivo
    T source, destination;
    int maxWalkTime;
    vector<T> avoidNodes;
    vector<pair<T, T>> avoidSegments;

    string line;
    while (getline(inputFile, line)) {
        size_t division = line.find(":");
        string key = line.substr(0, division);
        string value = line.substr(division + 1);

        if (key == "Mode") mode = value;
        else if (key == "Source") source = value;
        else if (key == "Destination") destination = value;
        else if (key == "MaxWalkTime") maxWalkTime = stoi(value);
        else if (key == "AvoidNodes") avoidNodes = getNumbers(value);
        else if (key == "AvoidSegments") {
            stringstream ss(value);
            string segment;
            while (getline(ss, segment, ')')) {
                size_t open = segment.find('(');
                size_t comma = segment.find(',', open + 1);

                T src = segment.substr(open + 1, comma - open - 1);
                T dest = segment.substr(comma + 1);

                avoidSegments.emplace_back(src, dest);
            }
        }
    }
    inputFile.close();

    // Marcar nós e segmentos a evitar
    for (const T &node : avoidNodes) {
        if (auto v = graph.findVertex(node)) v->setIgnore(true);
    }
    for (const auto &[src, dest] : avoidSegments) {
        if (auto e = graph.findEdge(src, dest)) e->setIgnore(true);
    }


    // Encontrar todos os nós de estacionamento válidos
    vector<Vertex<T> *> parkingNodes;
    for (auto vertex : graph.getVertexSet()) {
        if (vertex->getParking() && !vertex->isIgnore()) {
            // Verificar se o nó é acessível de carro e a pé
            dijkstra(&graph, source, "driving");
            if (getPath(&graph, source, vertex->getCode()).empty()) continue;

            dijkstra(&graph, vertex->getCode(), "walking");
            if (getPath(&graph, vertex->getCode(), destination).empty()) continue;

            parkingNodes.push_back(vertex);
        }
    }

    // Calcular tempos para cada nó de estacionamento
    struct ParkingOption {
        Vertex<T> *parking;
        int drivingTime;
        int walkingTime;
        int totalTime;
    };

    vector<ParkingOption> validOptions;
    for (auto parking : parkingNodes) {
        dijkstra(&graph, source, "driving");
        int drivingTime = graph.findVertex(parking->getCode())->getDist();

        dijkstra(&graph, parking->getCode(), "walking");
        int walkingTime = graph.findVertex(destination)->getDist();

        if (walkingTime <= maxWalkTime) {
            validOptions.push_back({parking, drivingTime, walkingTime, drivingTime + walkingTime});
        }
    }

    // Selecionar a melhor opção
    if (validOptions.empty()) {
        ofstream outputFile("output.txt");
        outputFile << "Source:" << source << endl;
        outputFile << "Destination:" << destination << endl;
        outputFile << "DrivingRoute:" << endl;
        outputFile << "ParkingNode:" << endl;
        outputFile << "WalkingRoute:" << endl;
        outputFile << "Message: No possible route with max. walking time of "<< maxWalkTime <<" minutes." << endl;
        outputFile.close();
        return;
    }

    auto bestOption = *min_element(validOptions.begin(), validOptions.end(), [](const ParkingOption &a, const ParkingOption &b) {
        if (a.totalTime != b.totalTime) return a.totalTime < b.totalTime;
        return a.walkingTime > b.walkingTime;
    });

    // Gerar o arquivo de saída
    ofstream outputFile("output.txt");
    outputFile << "Source:" << source << endl;
    outputFile << "Destination:" << destination << endl;

    // Rota de direção
    dijkstra(&graph, source, "driving");
    vector<T> drivingRoute = getPath(&graph, source, bestOption.parking->getCode());
    outputFile << "DrivingRoute:";
    for (size_t i = 0; i < drivingRoute.size(); i++) {
        int idx = graph.findVertexIdxCode(drivingRoute[i]);
        outputFile << graph.getVertexSet()[idx]->getId();
        if (i < drivingRoute.size() - 1) outputFile << ",";
    }
    outputFile << "(" << bestOption.drivingTime << ")" << endl;

    // Nó de estacionamento
    outputFile << "ParkingNode:" << bestOption.parking->getId() << endl;

    // Rota de caminhada
    dijkstra(&graph, bestOption.parking->getCode(), "walking");
    vector<T> walkingRoute = getPath(&graph, bestOption.parking->getCode(), destination);
    outputFile << "WalkingRoute:";
    for (size_t i = 0; i < walkingRoute.size(); i++) {
        int idx = graph.findVertexIdxCode(walkingRoute[i]);
        outputFile << graph.getVertexSet()[idx]->getId();
        if (i < walkingRoute.size() - 1) outputFile << ",";
    }
    outputFile << "(" << bestOption.walkingTime << ")" << endl;

    // Tempo total
    outputFile << "TotalTime:" << bestOption.totalTime << endl;
    outputFile.close();
}

