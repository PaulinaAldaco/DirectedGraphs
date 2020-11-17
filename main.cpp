#include"graph.h"
#include"utility_functions.h"

#include<iostream>
#include<fstream>
#include<vector>
#include<tuple>

using namespace std;

//Read contents of file containing instructions to create a graph
tuple<int,int,vector<vector<int>> > loadGraphFile(string fileName) {
    //Variables to read file
    string line;
    vector<vector<int>> adjacencies;
    int lineCounter = 1;
    int numberVertices,numberAdjacencies;

    ifstream myFile(fileName);
    //Read file line by line
    while(getline(myFile,line)) {
        if(lineCounter==1)
            //Number of vertices
            numberVertices = stoi(line);
        else if(lineCounter==2)
            //Number of adjacencies (arcs/edges)
            numberAdjacencies = stoi(line);
        else if(lineCounter>2 && lineCounter <= numberAdjacencies+2) {
            //Create a vector with the instructions to form the adjacencies
            //Use utility function to split the string into a vector of integers,
            //taking a comma as the delimiter
            adjacencies.push_back(splitStringToIntVect(line,","));
        }
        ++lineCounter;
    }
    myFile.close();

    //Return values as a tuple
    return make_tuple(numberVertices,numberAdjacencies,adjacencies);
}


int main() {
    //Number of files to be read
    int numberFiles = 4;
    //Variable for the name of the files
    string file;
    //Variables for number of vertices and adjacencies
    int numVert,numAdj;
    //Vector of adjacencies
    vector<vector<int>> adjs;

    //Read all files
    for(int i=1; i<=numberFiles; ++i) {
        //Get name of file to open
        file = "graph_" + to_string(i) + ".txt";
        //Read file and assign values of returned tuple
        tie(numVert,numAdj,adjs) = loadGraphFile(file);

        //Initialize graph and load adjacencies
        Graph newGraph(numVert);
        newGraph.loadGraph(numAdj,adjs);

        cout << "******** Graph " << i << " ********" << endl;
        //Show adjacency list
        cout << "Adjacency list:" << endl;
        newGraph.showAdjList();

        //Show adjacency matrix
        cout << endl;
        cout << "Adjacency matrix:" << endl;
        newGraph.showAdjMatrix();

        //Show if graph is tree
        cout << endl;
        cout << "Is it a tree?:" << endl;
        if(newGraph.isTree())
            cout << "IS A TREE" << endl;
        else
            cout << "IS NOT A TREE" << endl;

        //Show if graph is bipartite
        cout << endl;
        cout << "Is it bipartite?:" << endl;
        if(newGraph.isBipartite())
            cout << "IS BIPARTITE" << endl;
        else
            cout << "IS NOT BIPARTITE" << endl;

        //Show traversals
        cout << endl;
        cout << "---Traversals starting on vertex 0---" << endl;

        //Show Breadth First Search (Traversal)
        cout << endl;
        cout << "Breadth First Search:" << endl;
        newGraph.breadthFirstSeach(0);
        cout << endl;

        //Show Depth First Search (Traversal)
        cout << endl;
        cout << "Depth First Search:" << endl;
        newGraph.depthFirstSearch(0);
        cout << endl;

        //Show Topological Search (Traversal)
        cout << endl;
        //Topological search can only be done on acyclic graphs
        cout << "Topological Sort:" << endl;
        if(!newGraph.isCyclic()) {
            newGraph.topologicalSort();
            cout << endl;
        }
        else
            cout << "Cannot perform topological search: graph is cyclic" << endl;
        cout << endl << endl;

    }
    return 0;
}

