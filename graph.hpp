#ifndef _GRAPH_H
#define _GRAPH_H

#include <unordered_set>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Graph{
    private:
        // Adj. Matrix
        vector<vector<bool>> matrix;

        // Auxiliary vector to map the vertexes in order
        // Each element is represented by the number of connections and the real index (first and second element of the pair respectively)
        vector<pair<size_t,size_t>> sorted_vertex;
    public:

        // Constructor
        Graph(string file){
            ifstream f(file);

            if (f.is_open()){
                string line;
                size_t pos, nNodes, v1, v2;
                getline(f, line); // First line: Comment

                getline(f, line); // Second line: Get the number of nodes
                line.erase(0,7);
                pos = line.find(" ");
                nNodes = atoi(line.substr(0, pos).c_str());

                // Adj. Matrix Initialization
                matrix = vector<vector<bool>>(nNodes, vector<bool>(nNodes));
                
                // Initialization of sorted_vertex
                for (size_t i = 0; i < nNodes; i++){
                    sorted_vertex.push_back(make_pair(0, i));
                }

                while(getline(f, line)){ // Third+ lines: Create edges
                    line.erase(0, 2);
                    pos = line.find(" ");
                    v1 = atoi(line.substr(0, pos).c_str());
                    v2 = atoi(line.substr(pos+1, line.length()).c_str());

                    addEdge(v1, v2);

                    // Add a connection to the vertex
                    sorted_vertex[v1].first++;
                    sorted_vertex[v2].first++;
                }

                // Sort the vertexes
                sort(sorted_vertex.begin(), sorted_vertex.end(), greater<>());

            }else{
                cout << "Error: Could not open" << file << endl;
            }	
        }

        // Get the size of the graph (number of nodes)
        size_t size(){
            return matrix.size();
        }

        // Check if nodes x and y are connected
        bool areConnected(const size_t x, const size_t y){
            return matrix[x][y];
        }

        // Create edge between nodes x and y
        void addEdge(const size_t x, const size_t y){
            matrix[x][y] = matrix[y][x] = true;
        }

        // Remove the edge between nodes x and y
        void removeEdge(const size_t x, const size_t y){
            matrix[x][y] = matrix[y][x] = false;
        }

        // Get the n-node with greatest number of connections.
        size_t sortedVertex(size_t n){
            return sorted_vertex[n].second;
        }

        // Get all the nodes sorted by number of connections
        vector<size_t> getSortedVertexes(){
            vector<size_t> sortedVertexes;
            for (auto i : sorted_vertex)
                sortedVertexes.push_back(i.second);

            return sortedVertexes;
        }
};

#endif