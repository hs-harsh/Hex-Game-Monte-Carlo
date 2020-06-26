#include <iostream>
#include <ctime>
#include <queue>
#include <algorithm>
#include <fstream>
#include <set>

#include "Graph.h"

using namespace std;

/*****************************************************************************
Constructors
******************************************************************************/

// Creates a deep copy of another Graph
template <class Weight> Graph<Weight>::Graph(const Graph<Weight>& graph) : 
        noOfVertices(graph.noOfVertices), size(graph.size), 
        DISCONNECTED(graph.DISCONNECTED) {
    weights = new Weight[size];
    for (unsigned long i = 0; i < size; i++) {
        weights[i] = graph.weights[i];
    }
    if (graph.cachedKMST != NULL) {
         cachedKMST = new Graph<Weight>(*graph.cachedKMST);
    }
    if (graph.cachedPMST != NULL) {
         cachedPMST = new Graph<Weight>(*graph.cachedPMST);
    }
    changedKMST = graph.changedKMST;
    changedPMST = graph.changedPMST;
}

// Creates an empty Graph 
template <class Weight> Graph<Weight>::Graph(Weight disconnected, 
        unsigned int noOfVertices) : DISCONNECTED(disconnected) {
    init(noOfVertices);
}

template <class Weight> Graph<Weight>::Graph(Weight disconnected, string filename) :
        DISCONNECTED(disconnected) {
    // Read through the file
    string line;
    ifstream datafile(filename);
    
    if (datafile.is_open()) {
        unsigned int vertex1, vertex2;
        Weight weight;
        // First line is the number of vertices
        datafile >> noOfVertices;
        init(noOfVertices);
        // The remainder is a connection list
        while (datafile >> vertex1 >> vertex2 >> weight) {
                setWeight(vertex1, vertex2, weight);
        }
        datafile.close();
    } else {
        throw runtime_error("Could not open file");
    }
}

/* This constructor creates a undirectioned graph with the given arguments.
   noOfVertices:The number of vertices in the graph.
   edgeDensity: Indicates how many of the possible connections should exist
   and must be between 0 and 1 (inclusive) otherwise an out_of_range error
   will be thrown.
   min, max: The minimum and maximum weights between vertices,
   min must be non-negative and less than or equal to max otherwise an
   out_of_range exception is thrown
   Weight: Must be either char, short, or int
   fullyConnected: Creates a fully connected graph if true
*/
template <class Weight> Graph<Weight>::Graph(Weight disconnected,
        unsigned int noOfVertices, double edgeDensity,
        Weight min, Weight max, bool fullyConnected) :
        Graph(disconnected, noOfVertices) {
    
    // Calcuate size (number of edges if fully connected) and number of edges
    unsigned long noOfEdges = (unsigned long)(this->size * edgeDensity);

    // Check the input arguments are valid
    if (edgeDensity < 0 || edgeDensity > 1) {
        throw out_of_range("edgeDensity must be between 0 and 1 inclusive");
    }
    if (min < Weight(0) || min > max) {
        throw out_of_range("Input arguments must be 0 <= min <= max");
    }
    // Check if there are enough edges to create fully connected graph
    if (fullyConnected && noOfVertices - 1 > noOfEdges) {
        throw out_of_range("edgeDensity/noOfVertices too low to create a \
                                   fully connected graph");
    }

    // Populate weights
    // Initialize random seed based on current time
    srand(time(NULL));
    // Initially all edges are unallocated
    unsigned long unallocatedEdges = noOfEdges;

    // If requested, make a random fully connected graph
    if (fullyConnected) {
        // Create a shuffled array of vertices
        unsigned int* shuffled = new unsigned int[noOfVertices];
        for (unsigned int i = 0; i < noOfVertices; i++) {
            shuffled[i] = i;
        }
        random_shuffle(&shuffled[0], &shuffled[noOfVertices]);
        // Go through the array, indicies < i are parted of the connected
        // graph. Randomly choose part of the connected graph to connect
        // to the next unconnected index
        for (unsigned int i = 1; i < noOfVertices; i++) {
            // Select a random vertex in the connected graph (random index 
            // from 0 to i (exclusing i)) to connect to the next index.
            int cIndex = rand() % i;
            // Add the edge with a random weight within the range
            unallocatedEdges--;
            Weight weight = (Weight)(rand() % (max - min + 1) + min);
            setWeight(shuffled[i], shuffled[cIndex], weight);
        }
        // Remove shuffled array
        delete[] shuffled;
    }
    /* Randomly select the remaining possible edges:
      
       Go through all the unallocated edges and for each one randomly decide
       if it will exist. If so, choose a random weight within the range,
       otherwise, set the range to DISCONNECTED
      
       The probability of selecting any edge is:
          number_of_unallocated_edges / number_of_possible_edges
       which is in the loop below:
          unallocatedEdges / (size - i)
       note that both the number of unallocated edges, and possible edges
       may go down each iteration as we allocate them (or don't).
      
       The random probability is calculated as rand() / (RAND_MAX + 1) so
       that it is in the range 0 <= number < 1 such that if 
       number_of_unallocated_edges == number_of_possible_edges I force
       allocation.
      
       Below this has been rearranged as multiplication to avoid rounding
       errors, and cast to unsigned long multiplication to avoid overflow.
    */
    
    // Go through all possible edges
    for (unsigned long i = 0; i < size; i++) {
        // If it doesn't yet exist
        if (weights[i] == DISCONNECTED) {
            // Decide if this edge will exist
            if ((unsigned long)rand() * (size - i) < (unsigned long)
                unallocatedEdges * ((unsigned long)RAND_MAX + 1)) {
                // Select a random weight within the range
                unallocatedEdges--;
                weights[i] = (Weight)(rand() % (max - min + 1) + min);
            }
            else {
                // Unconnected edge
                weights[i] = Weight(DISCONNECTED);
            }
        }
    }
}


/*****************************************************************************
Deconstructors
******************************************************************************/

template <class Weight> Graph<Weight>::~Graph() {
    delete[] weights;
    delete cachedKMST;
    delete cachedPMST;
}



/*****************************************************************************
Public Methods
******************************************************************************/

// Returns weight between 2 vertices. <0 indicates they are not connected.
// The weight between a vertex and itself is 0
template<class Weight> Weight Graph<Weight>::getWeight(
    unsigned int vertex1, unsigned int vertex2) {
    if (vertex1 == vertex2) {
        return Weight(0);
    }
    else {
        return weights[getIndex(vertex1, vertex2)];
    }
}

// Returns an array of weights where the value corresponds to the weight
// between the argument vertex and the arrays index
template<class Weight> Weight* Graph<Weight>::getWeights(
    unsigned int vertex) {
    Weight* weights = new Weight[noOfVertices];
    for (unsigned int i = 0; i < noOfVertices; i++) {
        weights[i] = getWeight(vertex, i);
    }
    return weights;
}

// Sets weight between 2 vertices. weight <0 indicates they are not connected
template<class Weight> void Graph<Weight>::setWeight(
    unsigned int vertex1, unsigned int vertex2,
    Weight weight) {
    if (vertex1 == vertex2) {
        throw out_of_range("setWeight cannot be called with vertex1 == vertex2");
    }
    else {
        weights[getIndex(vertex1, vertex2)] = weight;
        if (cachedKMST != NULL) {
            cachedKMST->setWeight(vertex1, vertex2, weight);
            changedKMST = true;
        }
        if (cachedPMST != NULL) {
            cachedPMST->setWeight(vertex1, vertex2, weight);
            changedPMST = true;
        }
    }
}

// Returns true if the vertices are connected, directly or indirectly
template<class Weight> bool Graph<Weight>::isConnected(
        unsigned int vertex1, unsigned int vertex2, bool cache) {
    // If this method is to be performed on a small changing graph
    // use the MST to improve performance on large graphs.
    Graph* graph = cache ? getKruskalsMinimumSpanningTree() : this;
    set<unsigned int> pending;
    set<unsigned int> checked;
    pending.insert(vertex1);
    while (!pending.empty()) {
        set<unsigned int>::iterator itr = pending.begin();
        checked.insert(*itr);
        Weight* weights = graph->getWeights(*itr);
        pending.erase(itr);
        for(unsigned int vertex = 0; vertex < noOfVertices; vertex++) {
            if (weights[vertex] != DISCONNECTED) {
                if (vertex == vertex2) {
                    return true;
                }
                if (checked.find(vertex) == checked.end()) {
                    pending.insert(vertex);
                }
            }
        }
    }
    return false;
}

// Returns a new graph which the updated minimum spanning tree of this graph using
// Kruskal's algorithm
template<class Weight> Graph<Weight>*
Graph<Weight>::getKruskalsMinimumSpanningTree() {
    if (!changedKMST) {
        return cachedKMST;
    }
    Graph<Weight>* currentMST = (cachedKMST == NULL) ? this : cachedKMST;
    // Create a priority queue of edges
    priority_queue<Edge> edges;
    for (unsigned int i = 0; i < noOfVertices; i++) {
        for (unsigned int j = i + 1; j < noOfVertices; j++) {
            Weight weight = currentMST->getWeight(i, j);
            if (weight != DISCONNECTED) {
                edges.push(*(new Edge(i, j, weight)));
            }
        }
    }
    // Create an array where index corresponds to the vertex and the
    // value corresponds to the tree (initially there is 1 tree for each
    // vertex)
    unsigned int* trees = new unsigned int[noOfVertices];
    for (unsigned int i = 0; i < noOfVertices; i++) {
        trees[i] = i;
    }
    unsigned int noOfDiffTrees = noOfVertices;
    // Create empty graph copy of this one
    Graph<Weight>* graph = new Graph<Weight>(DISCONNECTED, noOfVertices);
    // Go through the edges priority queue
    while (!edges.empty() && noOfDiffTrees > 1) {
        // Get the next the edge
        Edge edge = edges.top();
        // If this edge connects 2 different trees
        unsigned int tree1 = trees[edge.vertex1];
        unsigned int tree2 = trees[edge.vertex2];
        if (tree1 != tree2) {
            // Add it to the new Graph
            graph->setWeight(edge.vertex1, edge.vertex2, edge.weight);
            // Join the separate trees
            for (unsigned int i = 0; i < noOfVertices; i++) {
                if (trees[i] == tree2) {
                    trees[i] = tree1;
                }
            }
            noOfDiffTrees--;
        }
        // Remove the edge from the priority queue
        edges.pop();
    }
    // Clean up
    delete[] trees;
    while (!edges.empty()){
        edges.pop();
    }
    // Return minimum spanning tree
    cachedKMST = graph;
    changedKMST = false;
    return graph;
}

// Returns a new graph which is the updated minimum spanning tree of this graph using
// Prims's algorithm
template<class Weight> Graph<Weight>*
Graph<Weight>::getPrimsMinimumSpanningTree() {
    if (!changedPMST) {
        return cachedPMST;
    }
    Graph<Weight>* currentMST = (cachedKMST == NULL) ? this : cachedPMST;
    // Create a priority queue of edges
    priority_queue<Edge> edges;
    // Create an array where index corresponds to the vertex and the
    // value indicates if the vertex is in the connected tree. Initially
    // only the first vertex is in the connected tree
    bool* connected = new bool[noOfVertices];
    connected[0] = true;
    for (unsigned int i = 1; i < noOfVertices; i++) {
        connected[i] = false;
    }
    // Count the remaining vertices to connect
    unsigned int unconnected = noOfVertices - 1;
    // Add all the connected edges to the priority queue (except connections
    // to itself)
    Weight* weights = currentMST->getWeights(0u);
    for (unsigned int i = 1; i < noOfVertices; i++) {
        if (weights[i] != DISCONNECTED) {
            edges.push(*(new Edge(0, i, weights[i])));
        }
    }
    delete weights;
    // Create empty graph copy of this one
    Graph<Weight>* graph = new Graph<Weight>(DISCONNECTED, noOfVertices);
    // Go through the edges priority queue
    while (!edges.empty() && unconnected > 0) {
        // Get and pop the next the edge
        Edge edge = edges.top();
        unsigned int vertex1 = edge.vertex1;
        unsigned int vertex2 = edge.vertex2;
        Weight weight = edge.weight;
        edges.pop();
        // If the new vertex isn't in the tree (the first must be otherwise
        // we wouldn't have added it to the queue)
        if (!connected[vertex2]) {
            // Add it to the new Graph
            graph->setWeight(vertex1, vertex2, weight);
            connected[vertex2] = true;
            unconnected--;
            // Add the new edges to the queue if they wouldn't cause a loop
            weights = currentMST->getWeights(vertex2);
            for (unsigned int i = 0; i < noOfVertices; i++) {
                if (!connected[i] && weights[i] != DISCONNECTED) {
                    edges.push(*(new Edge(vertex2, i, weights[i])));
                }
            }
            delete weights;
        }
    }
    // Clean up
    delete[] connected;
    while (!edges.empty()){
        edges.pop();
    }
    // Return minimum spanning tree
    cachedPMST = graph;
    changedPMST = false;
    return graph;
}

template<class Weight> void Graph<Weight>::display() {
    // Display column headers
    cout << "    ";
    for (unsigned int i = 0; i < this->noOfVertices; i++) {
        cout << "v" << i << "  ";
    }
    cout << endl;
    for (unsigned int i = 0; i < noOfVertices; i++) {
        // Display row headers
        cout << "v" << i << " |";
        for (unsigned int j = 0; j < noOfVertices; j++) {
            Weight weight = getWeight(i, j);
            // Display " - " it is not connected
            if (weight == DISCONNECTED) {
                cout << " - |";
            }
            else {
                cout << " " << (int)weight << " |";
            }
        }
        cout << endl;
    }
}

/*****************************************************************************
Private Methods
******************************************************************************/

// Initializes an empty Graph 
template<class Weight> void Graph<Weight>::init(
        unsigned int noOfVertices) {
    this->noOfVertices = noOfVertices;
    this->size = ((unsigned long)noOfVertices * (noOfVertices - 1)) / 2;
    // Setup weights array as completely unconnected
    this->weights = new Weight[size];
    for (unsigned long i = 0; i < size; i++) {
        weights[i] = DISCONNECTED;
    }
}

// Returns the index in weights array for the connection between the 
// specified vertices
template<class Weight> unsigned long Graph<Weight>::getIndex(
    unsigned int vertex1, unsigned int vertex2) {
    if (vertex1 == vertex2) {
        throw out_of_range("getIndex cannot be called with vertex1 == vertex2");
    }
    else {
        // This is undirectional graph so can assume vertex1 < vertex2 or make 
        // it so.
        if (vertex1 > vertex2) {
            unsigned int temp = vertex1;
            vertex1 = vertex2;
            vertex2 = temp;
        }
        // Return the index in weights array
        return (size - 1) - ((unsigned long)(noOfVertices - 1
            - vertex1) * ((noOfVertices - vertex1)) / 2) + (vertex2 - vertex1);
    }
}

/*****************************************************************************
Private Class Edge
******************************************************************************/

template<class Weight> Graph<Weight>::Edge::Edge(unsigned int vertex1, 
        unsigned int vertex2, Weight weight) :
        vertex1(vertex1), vertex2(vertex2), weight(weight){}

template<class Weight> bool Graph<Weight>::Edge::operator<(const Edge& rhs) const {
    return this->weight > rhs.weight;
}

/*****************************************************************************
Misc
******************************************************************************/
template class Graph<bool>;
template class Graph<char>;
template class Graph<int>;