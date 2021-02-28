#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

Graph *leitura(ifstream &input_file, int directed, int weightedEdge, int weightedNode){
    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph *graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if (!graph->getWeightedEdge() && !graph->getWeightedNode()){
        while (input_file >> idNodeSource >> idNodeTarget){
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
        }
    }else if (graph->getWeightedEdge() && !graph->getWeightedNode()){
        float edgeWeight;

        while (input_file >> idNodeSource >> idNodeTarget >> edgeWeight){
            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
        }
    }else if (graph->getWeightedNode() && !graph->getWeightedEdge()){
        float nodeSourceWeight, nodeTargetWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight){
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    }else if (graph->getWeightedNode() && graph->getWeightedEdge()){
        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight){
            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    }

    return graph;
}

Graph *leituraInstancia(ifstream &input_file, int directed, int weightedEdge, int weightedNode){
    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    //int numEdges;

    //Pegando a ordem do grafo

    //input_file >> order >> numEdges; // Isso não pega o numero de arestas, e o numero de arestas nem é utilizado para nada
    // por isso removi a variavel -k

    input_file >> order;

    //Criando objeto grafo
    Graph *graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while (input_file >> idNodeSource >> idNodeTarget){
        graph->insertEdge(idNodeSource, idNodeTarget, 0);
    }
    return graph;
}

int menu()
{

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Subgrafo induzido por conjunto de vértices" << endl;
    cout << "[2] Caminho Mínimo entre dois vértices - Dijkstra" << endl;
    cout << "[3] Caminho Mínimo entre dois vértices - Floyd" << endl;
    cout << "[4] Árvore Geradora Mínima de Prim" << endl;
    cout << "[5] Árvore Geradora Mínima de Kruskal" << endl;
    cout << "[6] Imprimir caminhamento em largura" << endl;
    cout << "[7] Imprimir ordenacao topológica" << endl;
    cout << "[8] Algoritmo Guloso" << endl;
    cout << "[9] Algoritmo Guloso Randomizado " << endl;
    cout << "[10] Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;
}

void selecionar(int selecao, Graph *graph, ofstream &output_file){
    switch (selecao){

    //Subgrafo induzido por um conjunto de vértices X;
    case 1:
    {
        output_file << "//Subgrafo induzido por vertices" << endl;
        vector<int> lista;
        lista.push_back(0);
        lista.push_back(7);
        lista.push_back(15);
        lista.push_back(9);
        lista.push_back(1);
        lista.push_back(21);
        lista.push_back(12);
        lista.push_back(14);
        lista.push_back(19);
        lista.push_back(20);
        Graph *indu = graph->getVertexInduced(lista);
        indu->imprimeGrafo(output_file);

        break;
    }
        //Caminho mínimo entre dois vértices usando Dijkstra;
    case 2:
    {
        output_file << "//Caminho minimo usando Dijkstra" << endl;
        int s = 10;
        int t = 8;
        output_file << endl;
        output_file << "// Distancia entre " << s << " e " << t << " é de " << graph->dijkstra(s, t) << endl;
        break;
    }

        //Caminho mínimo entre dois vértices usando Floyd;
    case 3:
    {
        output_file << "//Caminho minimo usando Floyd" << endl;
        int s = 10;
        int t = 8;
        output_file << endl;
        output_file << "// Distancia entre " << s << " e " << t << " é de " << graph->floydWarshall(s, t) << endl;
        break;
    }

        //AGM Prim;
    case 4:
    {

        output_file << "//Arvore geradora minima usando algoritmo de prim" << endl;
        Graph *grafo = graph->agmPrim();
        grafo->imprimeGrafo(output_file);

        break;
    }

        //AGM - Kruscal;
    case 5:
    {
        output_file << "//Kruscal" << endl;
        Graph* grafo = graph->agmKuskal();
        grafo->imprimeGrafo(output_file);

        break;
    }

        //Busca em largura;
    case 6:
    {
        output_file << "//Busca em largura" << endl;
        graph->breadthFirstSearch(output_file);
        break;
    }
        //Ordenação Topologica;
    case 7:
    {
        output_file << "//Ordenação topologica" << endl;

        break;
    }

    case 0:
    {

        cout << "Saindo" << endl;
        break;
    }
    case 8:
    {
        output_file << "//Case 8, só a impressão" << endl;
        graph->imprimeGrafo(output_file);
        break;
    }
    default:
    {
        cout << " Error!!! invalid option!!" << endl;
    }
    }
}

int mainMenu(ofstream &output_file, Graph *graph)
{

    int selecao = 1;

    while (selecao != 0){
        system("clear");
        selecao = menu();

        if (output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;
    }

    return 0;
}

/*
        VERSÃO DO PROFESSOR

int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if(input_file_name.find("v") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

     Graph* graph;

    if(input_file.is_open()){

        graph = leituraInstancia(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];

*/

//
//       VERSÃO PARA TESTES
//

int main()
{
    string program_name = "programa";
    string input_file_name = "entrada1-grafo.txt";

    ifstream input_file(input_file_name.c_str(), ios::in);
    ofstream output_file;
    //input_file.open(input_file_name,ios::in);
    output_file.open("saida.dot", ios::out | ios::trunc);

    Graph *graph;

    if (input_file.is_open()){

        graph = leitura(input_file, 0, 1, 0);
        // mudei de leituraInstancia para leitura, leitura lẽ grafos com pesos leitura instancia não
    }else{
        cout << "Unable to open ARQUIVO ENTRADA " << endl;
    }

    // DIFERENTE ATÉ AQUI
    //
    //
    //
    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}
