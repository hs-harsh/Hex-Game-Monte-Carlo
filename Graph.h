#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>

/* 
   This is an undirection, weighted graph. It is represented by an ordered 
   collection of vertices, where each pair of vertices has an associated 
   weight.
   
*/
template <class Weight> class Graph {
private:
    /* weights is an array which can hold all possible connections between 
       vertices. Note that I do not include any edge between a vertex and
       itself. 
       
       Since the graph is undirectional, an edge between vertex 1 and 
       vertex 2 implies the same edge between 2 and 1, thus I do not need 
       to store this information. I will only store the edge connecting the
       smaller vertex index to the greater.
      
       The array is the size of the max number of Edges:
           Sum of i from 1 to (noOfVertices - 1)
         = (noOfVertices * (noOfVertices - 1)) / 2
      
       For example, 4 vertices we could have edges graph:
          V1 V2 V3
       V0  x  x  x
          V1  x  x
              V2 x
      
       so max number of edges: 
          = 3 + 2 + 1
       which is the size of the weights array.
    */
    unsigned int noOfVertices;        // Number of vertices in Graph
    unsigned long size;               // Size of the weights array
    Weight* weights;                  // Weights between all directly 
                                      // connected vertices
    // If MST has been calculated for some subsection of an MST
    // then calculating the new MST is faster
    Graph* cachedKMST = NULL;   // Kruskals
    bool changedKMST = true;
    Graph* cachedPMST = NULL;   // Prims
    bool changedPMST = true;
    const Weight DISCONNECTED;  // For disconnected vertices
public:
    // Creates a deep copy of another graph
    Graph(const Graph<Weight>& graph);
    // Creates an empty Graph
    Graph(Weight disconnected, unsigned int noOfVertices);
    /* Creates a Graph using the connections in the file.
       The file format is:
           First line contains the number of vertices
           Remaining lines contain "vertex1 vertex2 weight"
           e.g.
           3
           0 1 7
           0 2 2
           1 2 9
    */ 
    Graph(Weight disconnected, std::string filename);
    // Creates a randomly connected graph
    Graph(Weight disconnected, unsigned int noOfVertices, double edgeDensity, 
            Weight min, Weight max, bool fullyConnected);
    ~Graph();
    // Returns the direct weight between the vertices
    Weight getWeight(unsigned int vertex1, unsigned int vertex2);
    // Returns an array of weights between the specified vertex and all others
    Weight* getWeights(unsigned int vertex);
    void setWeight(unsigned int vertex1, unsigned int vertex2, Weight weight);
    /* Returns true if any vertex1s are connected to vertex2s, directly or indirectly
       Set cache to true if this method will be called multiple times on the same
       (possibly changing) large graph to improve performance
    */
    bool isConnected(unsigned int vertex1, unsigned int vertex2, bool cache = false);
    // Returns a new graph which the a minimum spanning tree of this graph using
    // Kruskal's algorithm
    Graph* getKruskalsMinimumSpanningTree();
    // Returns a new graph which is a minimum spanning tree of this graph using
    // Prims's algorithm
    Graph*getPrimsMinimumSpanningTree();
    void display();
private:
    // Initializes as empty graph of required size
    void init(unsigned int noOfVertices);
    // Returns the index in weights array for the connection between the 
    // specified vertices
    unsigned long getIndex(unsigned int vertex1, unsigned int vertex2);
    /* This Edge's call is used in the priority queues in Kruskals and Prims
       minimum spanning tree methods: getKruskalsMinimumSpanningTree,
       getPrimsMinimumSpanningTree.
    */
    class Edge {
    public:
        unsigned int vertex1;
        unsigned int vertex2;
        Weight weight;
        Edge(unsigned int vertex1, unsigned int vertex2, Weight weight);
        bool operator<(const Edge& rhs) const;
    };
};

#endif  // GRAPH_H_