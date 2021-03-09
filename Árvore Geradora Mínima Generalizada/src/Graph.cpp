#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <algorithm>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{
    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{
    return this->order;
}

int Graph::getNumberEdges()
{
    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{
    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{
    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{
    return this->weighted_node;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    //k
    if (!searchNode(id))
    {
        if (this->first_node != nullptr)
        {
            Node *no = new Node(id);
            this->last_node->setNextNode(no);
            this->last_node = no;
        }
        else
        {
            Node *no = new Node(id);
            this->first_node = no;
            this->last_node = this->first_node;
        }
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    //k
    Node *aux1 = getNode(id);
    Node *aux2 = getNode(target_id);

    //verifica se os 2 vertices existem, e os cria se n�o existirem
    if (aux1 == nullptr)
    {
        insertNode(id);
        aux1 = getNode(id);
    }

    if (aux2 == nullptr)
    {
        insertNode(target_id);
        aux2 = getNode(target_id);
    }

    aux1->insertEdge(target_id, weight);
    aux1->incrementOutDegree(); // n�o tenhno certeza se o incremento do grau dos nos est� correto
    if (!this->directed)
    {
        aux2->insertEdge(id, weight);
    }
    else
    {
        aux2->incrementInDegree();
    }
}

void Graph::removeNode(int id)
{

    //TODO
}

bool Graph::searchNode(int id)
{
    //k
    Node *no = first_node;
    while (no != nullptr)
    {
        if (no->getId() == id)
        {
            return true;
        }
        no = no->getNextNode();
    }


    return false;
}

Node *Graph::getNode(int id)
{
    //k
    Node *no = first_node;
    while (no != nullptr)
    {
        if (no->getId() == id)
        {
            return no;
        }
        no = no->getNextNode();
    }
    return no;
}

//Function that prints a set of edges belongs breadth tree

void Graph::breadthFirstSearch(ofstream &output_file)
{
    //k
    //criar uma fila com todos os adj do vertice que estou olhando
    //criar uma lista com todos os vertices j� visitados
    queue<int> fila;
    vector<int> visitados;
    Edge *aresta;
    Node *aux;

    //imprime header do grafo no arquivo de saida
    if (this->directed)
    {
        output_file << endl
                    << "strict digraph bfs {" << endl
                    << "graph [overlap=false];" << endl; // para quando gerar o grafo no graphviz os vertices n�o
        // ficarem em cima um dos outros
    }
    else
    {
        output_file << endl
                    << "strict graph bfs {" << endl
                    << "graph [overlap=false];" << endl; // para quando gerar o grafo no graphviz os vertices n�o
        // ficarem em cima um dos outros
    }

    // coloca o primeiro vertice do grafo na fila
    fila.push(this->first_node->getId());

    while (!fila.empty())
    {
        aux = this->getNode(fila.front());
        fila.pop();
        visitados.push_back(aux->getId());
        aresta = aux->getFirstEdge();

        while (aresta != nullptr)
        {
            bool jahVisitado = false;
            for (int i = 0; i < visitados.size(); i++)
            {
                if (visitados[i] == aresta->getTargetId())
                {
                    jahVisitado = true;
                    break;
                }
            }
            if (!jahVisitado)
            {
                fila.push(aresta->getTargetId());
            }
            if (this->directed)
            {
                output_file << aux->getId() << " -> " << aresta->getTargetId() << ";" << endl;
            }
            else
            {
                output_file << aux->getId() << " -- " << aresta->getTargetId() << ";" << endl;
            }
            aresta = aresta->getNextEdge();
        }
    }

    output_file << "}" << endl;
}

float Graph::floydWarshall(int idSource, int idTarget)
{
    //k

    float inf = INFINITY;

    if (!this->directed || !this->weighted_edge)
    {
        cerr << "grafo n�o direcionado ou vertices n�o possuem peso" << endl;
        return inf;
    }

    if (!this->searchNode(idSource) || !this->searchNode(idTarget))
    {
        cerr << "Os 2 vertices n�o existem no grafo" << endl;
        return inf;
    }

    // matriz das distancias

    float matriz[this->order][this->order];

    // inicia a distancia de um vertice a ele mesmo como 0 e os outras distancias como infinito
    for (int i = 0; i < this->order; i++)
    {
        for (int j = 0; j < this->order; j++)
        {
            if (i == j)
            {
                matriz[i][j] = 0;
            }
            else
            {
                matriz[i][j] = inf;
            }
        }
    }

    Node *no = this->getFirstNode();
    int indexSource, indexTarget;

    // inicia a distancia dos vertices aos vizinhos como o peso da aresta
    for (int i = 0; i < this->order; i++)
    {
        Edge *aresta = no->getFirstEdge();

        //armazena o index do n� de entrada e o n� de saida na matriz
        if (no->getId() == idSource)
        {
            indexSource = i;
        }
        if (no->getId() == idTarget)
        {
            indexTarget = i;
        }

        //preenche a matriz com o peso de todas as arestas
        while (aresta != nullptr)
        {
            Node *aux = this->getFirstNode();
            int j;
            //procura o indice do target da aresta para incluir o peso da aresta na matriz
            for (j = 0; aux->getId() != aresta->getTargetId(); j++)
            {
                aux = aux->getNextNode();
            }
            matriz[i][j] = aresta->getWeight();
            aresta = aresta->getNextEdge();
        }
        no = no->getNextNode();
    }

    for (int k = 0; k < this->order; k++)
    {
        for (int i = 0; i < this->order; i++)
        {
            for (int j = 0; j < this->order; j++)
            {
                if (matriz[i][j] > matriz[i][k] + matriz[k][j])
                {
                    matriz[i][j] = matriz[i][k] + matriz[k][j];
                }
            }
        }
    }

    // imprime a matriz para testes
    /*
    for (int i = 0; i < this->order; i++)
    {
        for (int j = 0; j < this->order; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
    */
    return matriz[indexTarget][indexSource];
}

float Graph::dijkstra(int idSource, int idTarget)
{
    // k
    float inf = INFINITY;

    if (!this->directed || !this->weighted_edge)
    {
        cerr << "grafo n�o direcionado ou vertices n�o possuem peso" << endl;
        return inf;
    }

    if (!this->searchNode(idSource) || !this->searchNode(idTarget))
    {
        cerr << "Os 2 vertices n�o existem no grafo" << endl;
        return inf;
    }
    // estrutura de apoio para a procura do caminho
    struct No
    {
        float distancia;
        int idvertice;
        bool visitado;
        No(float distancia, int vertice) : distancia(distancia), idvertice(vertice), visitado(false)
        {
        }
    };

    No *no = new No(0, idSource);

    vector<No *> lista;
    lista.push_back(no);
    int idMenorDistancia;

    while (!lista.empty() || lista[idMenorDistancia]->idvertice != idTarget)
    {
        //procura o no n�o visitado com a menor distancia
        idMenorDistancia = 0;
        for (int i = 0; i < lista.size(); i++)
        {
            if (lista[idMenorDistancia]->visitado && !lista[i]->visitado)
            {
                idMenorDistancia = i;
            }
            if (!lista[idMenorDistancia]->visitado && !lista[i]->visitado && (lista[i]->distancia < lista[idMenorDistancia]->visitado))
            {
                idMenorDistancia = i;
            }
        }
        //verifica se todos os n�s j� n�o foram visitados
        if (idMenorDistancia == 0 && lista[0]->visitado == true)
        {
            break;
        }

        Edge *aresta = this->getNode(lista[idMenorDistancia]->idvertice)->getFirstEdge();

        //percorre todas as arestas de um vertice
        while (aresta != nullptr)
        {
            bool existe = false;
            int posLista;
            //verifica se o vertice j� existe na lista
            for (int i = 0; i < lista.size(); i++)
            {
                if (lista[i]->idvertice == aresta->getTargetId())
                {
                    existe = true;
                    posLista = i;
                    break;
                }
            }
            //se o vertice j� existir atualiza sua distancia, se necessario
            if (existe)
            {
                if (lista[posLista]->distancia > lista[idMenorDistancia]->distancia + aresta->getWeight())
                {
                    lista[posLista]->distancia = lista[idMenorDistancia]->distancia + aresta->getWeight();
                }
            }
            //caso o vertice n�o exista na lista, adiciona-o a lista
            else
            {
                no = new No(lista[idMenorDistancia]->distancia + aresta->getWeight(), aresta->getTargetId());
                lista.push_back(no);
            }
            aresta = aresta->getNextEdge();
        }
        //marca o vertice como visitado
        lista[idMenorDistancia]->visitado = true;
    }

    //se a lista for vazia  ent�o o grafo � desconexo (?? � esse o termo?)
    if (!lista.empty())
    {
        //procura o indice na lista do vertice alvo, para retornar sua distancia
        for (int i = 0; i < lista.size(); i++)
        {
            if (lista[i]->idvertice == idTarget)
            {
                idMenorDistancia = i;
            }
        }

        float dist = lista[idMenorDistancia]->distancia;

        //exclui os ponteiros dos No
        for (int i = 0; i < lista.size(); i++)
        {
            No *no = lista[i];
            delete no;
        }
        lista.clear();

        return dist;
    }
    else  // se a lista estiver vazia retorna infinito, pois n�o existe caminho entre os 2 vertices{
        return inf;
}


//function that prints a topological sorting
void topologicalSorting()
{
    //TODO
}

void breadthFirstSearch(ofstream &output_file)
{
    // repetido??  existe uma fun��o com o mesmo nome em cima
    //TODO
}
Graph *Graph::getVertexInduced(vector<int> listIdNodes)
{
    // k
    Graph *grafoInduzido = new Graph(listIdNodes.size(), this->directed, this->weighted_edge, this->weighted_node);

    // insere todos os vertices da lista no grafo induzido
    for (int i = 0; i < listIdNodes.size(); i++)
    {
        if (this->searchNode(listIdNodes[i]))
        {
            grafoInduzido->insertNode(listIdNodes[i]);
        }
    }

    // percorre todas as arestas dos vertices do grafo original, e adiciona ao induzido as arestas necessarias
    Node *noaux1 = grafoInduzido->first_node;
    while (noaux1 != nullptr)
    {
        Node *noaux2 = this->getNode(noaux1->getId());

        Edge *aresta = noaux2->getFirstEdge();
        while (aresta != nullptr)
        {
            if (grafoInduzido->searchNode(aresta->getTargetId()))
            {
                grafoInduzido->insertEdge(noaux1->getId(), aresta->getTargetId(), aresta->getWeight());
            }
            aresta = aresta->getNextEdge();
        }

        noaux1 = noaux1->getNextNode();
    }

    return grafoInduzido;
}

//auxiliar para kuskal
struct par
{
    int idOrigem;
    Edge *aresta;
    par(int id, Edge *aresta) : idOrigem(id), aresta(aresta)
    {
    }
};

bool comp(par *a, par *b) // func auxiliar para kuskal
{
    return a->aresta->getWeight() < b->aresta->getWeight();
}

Graph *Graph::agmKuskal()
{
    //TODO

    // pegar as arestas em ordem crescente de peso
    // a aresta s� pode ser adicionada se ambos os vertices n�o j� pertencerem ao conjunto de arestas
    //fazer isso at� adicionar todos os vertices
    if (!this->weighted_edge && this->directed)
    {
        cerr << "Erro : Arestas n�o possuem peso ou o grafo � direcionado" << endl;
        return nullptr;
    }
    Graph *grafo = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);
    vector<par *> lista;
    Node *no = this->first_node;
    while (no != nullptr)
    {
        Edge *aresta = no->getFirstEdge();
        while (aresta != nullptr)
        {
            lista.push_back(new par(no->getId(), aresta));
            aresta = aresta->getNextEdge();
        }
        no = no->getNextNode();
    }

    sort(lista.begin(), lista.end(), comp);

    for(int pos =0; pos< lista.size(); pos++ )
    {
        if(!grafo->searchNode(lista[pos]->idOrigem) || !grafo->searchNode(lista[pos]->aresta->getTargetId()))
        {
            grafo->insertEdge(lista[pos]->idOrigem, lista[pos]->aresta->getTargetId(), lista[pos]->aresta->getWeight());
        }
        if(grafo->order == this->order)
        {
            break;
        }
    }

    for(int i =0; i<lista.size(); i++)
    {
        delete lista[i];
    }
    lista.clear();

    return grafo;
}
Graph *Graph::agmPrim()
{
    if (!this->weighted_edge && this->directed)
    {
        cerr << "Erro : Arestas n�o possuem peso ou o grafo � direcionado" << endl;
        return nullptr;
    }
    vector<int> visitados;
    Graph *grafo = new Graph(this->order, this->directed, this->weighted_edge, this->weighted_node);

    visitados.push_back(this->first_node->getId());

    while (visitados.size() != this->order)
    {
        int idEdgeMinS;
        int idEdgeMinT;
        float pesoMin = INFINITY;

        for (int i = 0; i < visitados.size(); i++)
        {
            Node *aux = this->getNode(visitados[i]);
            Edge *aresta = aux->getFirstEdge();
            while (aresta != nullptr)
            {
                bool visit = false;
                if (aresta->getWeight() < pesoMin)
                {
                    for (int j = 0; j < visitados.size(); j++)
                    {
                        if (aresta->getTargetId() == visitados[j])
                        {
                            visit = true;
                            break;
                        }
                    }
                    if (!visit)
                    {
                        idEdgeMinS = aux->getId();
                        idEdgeMinT = aresta->getTargetId();
                        pesoMin = aresta->getWeight();
                    }
                }

                aresta = aresta->getNextEdge();
            }
        }
        grafo->insertEdge(idEdgeMinS, idEdgeMinT, pesoMin);
        visitados.push_back(idEdgeMinT);
    }
    return grafo;
}

//
// verificar e fazer as altera��es da classe busca
/*
void Graph::buscaLargura()
{
    void bfs(int v);
    queue<int> fila;
    bool visitados[V]; // vetor de visitados

    for(int i = 0; i < V; i++)
        visitados[i] = false;

    cout << "Visitando vertice " << v << " ...\n";
    visitados[v] = true; // marca como visitado

    while(true)
    {
        list<int>::iterator it;
        for(it = adj[v].begin(); it != adj[v].end(); it++)
        {
            if(!visitados[*it])
            {
                cout << "Visitando vertice " << *it << " ...\n";
                visitados[*it] = true; // marca como visitado
                fila.push(*it); // insere na fila
            }
        }

        // verifica se a fila N�O est� vazia
        if(!fila.empty())
        {
            v = fila.front(); // obt�m o primeiro elemento
            fila.pop(); // remove da fila
        }
        else
            break;
    }

}*/

// verificar e fazer as altera��es da classe busca

deque<Vertice> ordenacaoTopologica(Graph& grafo)
{

    map<string, bool> visitados;
    deque<Vertice> fila_ordenacao;

    for(Vertice& v: grafo.vertices)
        visitados[v.getNome()] = false;

    for(Vertice& v: grafo.vertices)
        if (!visitados[v.getNome()])
            ordenacaoTopologicaAux(v, visitados, fila_ordenacao);

    return fila_ordenacao;
}
void Graph::ordenacaoTopologica()
{
    //void ordenacaoTopologicaAux(Vertice& v, map<string,bool>& visitados, deque<Vertice>& fila_ordenacao) {

    visitados[v.getNome()] = true;

    for(Vertice* w: v.adjacentes)
        if (!visitados[w->getNome()])
            ordenacaoTopologicaAux(*w, visitados, fila_ordenacao);

    fila_ordenacao.push_front(v);
}



//
void Graph::imprimeGrafo(ofstream &output_file) // imprime o grafo no formato dot{
Node *no = this->first_node;
Edge *aresta = no->getFirstEdge();
if (this->directed)
{
    output_file << endl
                << "strict digraph impressao {" << endl
                << "graph [overlap=false];" << endl; // para quando gerar o grafo no graphviz os vertices n�o
    // ficarem em cima um dos outros
}
else
{
    output_file << endl
                << "strict graph impressao {" << endl
                << "graph [overlap=false];" << endl; // para quando gerar o grafo no graphviz os vertices n�o
    // ficarem em cima um dos outros
}
while (no != nullptr)
{
    aresta = no->getFirstEdge();
    if (aresta == nullptr)
    {
        output_file << no->getId() << ";" << endl;
    }
    else
    {
        while (aresta != nullptr)
        {
            if (this->directed)
            {
                output_file << no->getId() << " -> " << aresta->getTargetId() << ";" << endl;
            }
            else
            {
                output_file << no->getId() << " -- " << aresta->getTargetId() << ";" << endl;
            }
            aresta = aresta->getNextEdge();
        }
    }
    no = no->getNextNode();
}
output_file << "}" << endl;
}
