#ifndef GRAPH_
#define GRAPH_

#include<iostream>
#include<list>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

class Graph {

private:
    //Number of vertices in the graph
    int number_vertices_;
    //Number of adjacencies in the graph
    int number_adj_;
    //Adjacency list
    list<int> *adj_list_;
    //Adjacency matrix
    vector<vector<int>> adj_matrix_;

    //Helper function for depthFirstSearch
    //Recursive function to visit adjacent vertices of current vertex,
    //and their respective adjacent vertices
    void recursiveDepthFirstSearch(int currentVertex, bool visited[]) {
        //Mark current vertex as visited and print
        visited[currentVertex] = true;
        cout << currentVertex << " ";

        //Recur for all adjacent vertices that have not been visited yet
        list<int>::iterator i;
        for(i=adj_list_[currentVertex].begin(); i!=adj_list_[currentVertex].end(); ++i) {
            if(!visited[*i])
                recursiveDepthFirstSearch(*i,visited);
        }
    }

    //Helper function for isCyclic
    //Recursive function to check if there is a cycle
    //Visits vertices similarly to recursiveDepthFirstSearch, but keeps track of the
    //recursion stack to detect if a vertex on the current path has already been visited
    bool recursiveIsCyclic(int currentVertex, bool visited[], bool *recursionStack) {
        //If the current vertex hasn't been visited
        if(!visited[currentVertex]) {
            // Mark current vertex as visited and part of recursion stack
            visited[currentVertex] = true;
            recursionStack[currentVertex] = true;

            //Recur for all adjacent vertices
            list<int>::iterator i;
            for(i=adj_list_[currentVertex].begin(); i!=adj_list_[currentVertex].end(); ++i) {
                //If the vertex hasn't been visited, call recursive function and check for cycle
                if(!visited[*i] && recursiveIsCyclic(*i,visited,recursionStack))
                    return true;
                //If the vertex has been visited, and it's currently in the recursion stack,
                //then there is a cycle
                else if(recursionStack[*i])
                    return true;
            }
        }
        //Remove current vertex from recursionStack
        recursionStack[currentVertex] = false;

        //No cycle was found, so return false
        return false;
    }

    //Helper function for topologicalSort
    //Recursive function similar to recursiveDepthFirstSearch
    //Visits all adjacent vertices of current vertex and their respective adjacent vertices
    //Pushes the current vertex to the stack only when all its adjacent vertices are already
    //in the stack
    void recursiveTopologicalSort(int currentVertex, bool visited[], stack<int>& topologicalStack) {
        //Mark current vertex as visited
        visited[currentVertex] = true;

        //Recur for all adjacent vertices
        list<int>::iterator i;
        for(i=adj_list_[currentVertex].begin(); i!=adj_list_[currentVertex].end(); ++i) {
            //Only recur if it hasn't been visited
            if(!visited[*i])
                recursiveTopologicalSort(*i, visited, topologicalStack);
        }

        //Push current vertex to stack
        topologicalStack.push(currentVertex);
    }

    //Helper function for isBipartite
    //Traverses graph similarly to a breadth first search,
    //marking all adjacent vertices of current vertex with the opposite color
    //If this is not possible, the graph is not bipartite
    bool isBipartiteHelper(int firstVertex, int colors[]) {
        //Mark starting vertex as 1
        colors[firstVertex] = 1;

        //Queue to store visited vertices
        queue<int> q;
        //Enqueue vertex
        q.push(firstVertex);

        //Iterator for getting adjacent vertices
        list<int>::iterator i;

        //While the queue is not empty
        while(!q.empty()) {
            //Get first Vertex and then dequeue it
            firstVertex = q.front();
            q.pop();

            //Get all adjacent vertices of current vertex
            for(i=adj_list_[firstVertex].begin(); i!=adj_list_[firstVertex].end(); ++i) {
                //If the adjacent vertex hasn't been visited,
                //enqueue it and mark it with the opposite color of current vertex
                if(colors[*i] == -1) {
                    q.push(*i);
                    colors[*i] = 1- colors[firstVertex];
                }
                //If the adjacent vertex has already been visited and it's the same
                //color as current vertex, then the graph is not bipartite
                else if(colors[firstVertex] == colors[*i])
                    return false;
            }
        }
        //Otherwise, all adjacent vertices can be colored with a different color,
        //so the graph is bipartite
        return true;
    }

public:
    //Constructor
    Graph(int number_vertices) {
        //Set number of vertices
        number_vertices_ = number_vertices;
        //Set number of adjacencies to 0 (adjacencies haven't been loaded)
        number_adj_ = 0;
        //Create a list of size number_vertices
        adj_list_ = new list<int>[number_vertices];
        //Create a matrix of size number_vertices x number_vertices and fill it with zeros
        for(int i=0; i<number_vertices; ++i) {
            adj_matrix_.push_back(vector<int>(number_vertices,0));
        }
    }

    //Insert adjacency in adjacency list
    void insertAdjacency(int vertex, int adj_vertex) {
        //Only insert if adjacency doesn't already exist
        if(adj_matrix_[vertex][adj_vertex] != 1) {
            adj_list_[vertex].push_back(adj_vertex);
            //Update adjacency matrix
            updateAdjacencyMatrix(vertex,adj_vertex);
            //Increment number of adjacencies
            ++number_adj_;
        }
    }

    //Update matrix which represents adjacencies
    void updateAdjacencyMatrix(int vertex, int adj_vertex) {
        //Place a 1 at position [vertex][adj_vertex] to represent the adjacency between the vertices
        adj_matrix_[vertex][adj_vertex] = 1;
    }

    //Traverse list and print,
    //showing every vertex and its adjacent vertices
    void showAdjList() {
        for(int i=0; i<number_vertices_; ++i) {
            cout << i << "->";
            //Iterator for vertices
            list<int>::iterator j;
            for (j=adj_list_[i].begin(); j!=adj_list_[i].end(); ++j)
                cout << *j << ",";
            cout << "\n";
        }
    }

    //Traverse matrix and print
    void showAdjMatrix() {
        for(int i=0; i<number_vertices_; ++i) {
            for(int j=0; j<number_vertices_; ++j)
                cout << adj_matrix_[i][j];
            cout << "\n";
        }
    }

    //Insert all adjacencies to graph
    //number_adj = number of adjacencies (arcs/edges)
    //adj = adjacencies to be loaded onto graph
    void loadGraph(int number_adj, vector<vector<int>> adj) {
        for(int i=0; i<number_adj; ++i) {
            insertAdjacency(adj[i][0],adj[i][1]);
        }
    }

    //Function used by isTree() to check if graph is cyclic
    bool isCyclic(bool visited[]) {
        //Create a boolean array for vertices in recursion stack
        bool *recursionStack = new bool[number_vertices_];
        for(int i=0; i<number_vertices_; ++i)
            recursionStack[i] = false;

        //Call the recursive function to detect cycles for each vertex
        for(int i=0; i<number_vertices_; ++i) {
            //If there is a cycle return true
            if(recursiveIsCyclic(i,visited,recursionStack))
                return true;
        }

        //Otherwise return false
        return false;
    }

    //Function to simply check if graph is cyclic
    bool isCyclic() {
        //Create a boolean array and mark all vertices as not visited
        //Create a boolean array for vertices in recursion stack
        bool *visited = new bool[number_vertices_];
        bool *recursionStack = new bool[number_vertices_];
        for(int i=0; i<number_vertices_; ++i) {
            visited[i] = false;
            recursionStack[i] = false;
        }

        //Call the recursive function to detect cycles for each vertex
        for(int i=0; i<number_vertices_; ++i) {
            //If there is a cycle return true
            if(recursiveIsCyclic(i,visited,recursionStack))
                return true;
        }

        //Otherwise return false
        return false;
    }

    //Check if graph is a tree
    //To be a tree the number of adjacencies in the graph must be equal to
    //the number of vertices minus 1
    //This also implicitly checks if the graph is disconnected, since the number
    //adjacencies in a disconnected graph will be at least 2 less than the number
    //of vertices
    //Then check if the graph is cyclic, since it must not be cyclic to be a tree
    bool isTree() {
        //First check if the rule for trees is true:
        //number of adjacencies must be number of vertices minus 1
        if(number_adj_ == number_vertices_ - 1) {
            //Create a boolean array and mark all vertices as not visited
            bool *visited = new bool[number_vertices_];
            for(int i=0; i<number_vertices_; ++i)
                visited[i] = false;

            //Check if graph is cyclic starting on vertex 0
            //If it's cyclic, then it's not a tree
            if(isCyclic(visited)) {
                cout << "Graph is cyclic" << endl;
                return false;
            }

            //Otherwise return true
            return true;
        }
        //If number of adjacencies does not equal number of vertices minus 1,
        //it cannot be a tree
        else {
            cout << "Number of adjacencies is not equal to number of vertices minus 1" << endl;
            return false;
        }
    }

    //Perform a traversal similar to breadth first search, assigning one of two
    //"colors" to the vertices to differentiate between two sets
    //The colors will be represented with the numbers 1 and 0
    bool isBipartite() {
        //Create an array used to assign colors to vertices,
        //and to keep track of visited vertices
        //Assign -1 to all not visited vertices
        int colors[number_vertices_];
        for(int i=0; i<number_vertices_; ++i)
            colors[i] = -1;

        //Perform process for every vertex,
        //ensuring that all vertices will be visited
        for(int i=0; i<number_vertices_; ++i) {
            //Only perform process if vertex hasn't been processed
            if(colors[i]==-1)
                //Check adjacent vertices, and if not bipartite return false
                if(!isBipartiteHelper(i,colors))
                    return false;
        }
        //Otherwise return true
        return true;
    }

    //Breadth First Search (Traversal)
    //Uses a "neighbors" approach: it visits the starting vertex, marks it as visited,
    //enqueues it, prints it, dequeues it, and then visits all the adjacent vertices,
    //marks them as visited, and enqueues them only if they weren't visited yet. It then
    //repeats the process of showing, dequeuing, and getting the adjacent vertices of
    //the first vertex in the queue, until the queue is empty
    void breadthFirstSeach(int firstVertex) {
        //Create a boolean array and mark all vertices as not visited
        bool *visited = new bool[number_vertices_];
        for(int i=0; i<number_vertices_; ++i)
            visited[i] = false;

        //Queue to store visited vertices
        queue<int> q;

        //Mark starting vertex as visited and enqueue it
        visited[firstVertex] = true;
        q.push(firstVertex);

        //Iterator for getting adjacent vertices
        list<int>::iterator i;

        //While the queue is not empty
        while(!q.empty()) {
            //Get first vertex and print it
            firstVertex = q.front();
            cout << firstVertex << " ";
            //Dequeue vertex
            q.pop();

            //Visit all adjacent vertices of current vertex
            for(i=adj_list_[firstVertex].begin(); i!=adj_list_[firstVertex].end(); ++i) {
                //If the vertex hasn't been visited, enqueue it and mark as visited
                if(!visited[*i]) {
                    q.push(*i);
                    visited[*i] = true;
                }
            }
        }
    }

    //Depth First Search (Traversal)
    //Uses a "path" approach: it uses a recursive function to to go
    //down the a path starting in the first vertex until it reaches a
    //dead end or a vertex that has already been visited. Then it repeats
    //the process for all the vertices in the "path"
    void depthFirstSearch(int firstVertex) {
        //Create a boolean array and mark all vertices as not visited
        bool *visited = new bool[number_vertices_];
        for(int i=0; i<number_vertices_; ++i)
            visited[i] = false;

        //Call recursive helper function
        recursiveDepthFirstSearch(firstVertex,visited);
    }

    //Topological Sort (Traversal)
    //This function is similar to depth first search, but stores
    //vertices in a stack and prints them at the end
    void topologicalSort() {
        //Create a boolean array and mark all vertices as not visited
        bool *visited = new bool[number_vertices_];
        for(int i=0; i<number_vertices_; ++i)
            visited[i] = false;

        //Stack to store visited vertices
        stack<int> topologicalStack;

        //Call recursive function for all vertices
        for(int i=0; i<number_vertices_; ++i) {
            //Only call is it hasn't been visited
            if(!visited[i])
                recursiveTopologicalSort(i,visited,topologicalStack);
        }

        //Print stack
        while(!topologicalStack.empty()) {
            cout << topologicalStack.top() << " ";
            topologicalStack.pop();
        }
    }
};

#endif
