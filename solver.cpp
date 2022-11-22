#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <set>

#include "graph.hpp"
#include "generator.hpp"
#include "translator.hpp"


using namespace std::chrono;

bool check_solution(Graph& G, const vector<size_t>& sol){
    for (size_t i = 0; i < G.size(); i++){
        for (size_t j = i + 1; j < G.size(); j++){
            if (sol[i] == sol[j] and G.areConnected(i, j))
                return false;
        }
    }
    return true;
}

bool check_solution_v2(Graph& G, const vector<size_t>& sol){
    for (size_t i = 0; i < G.size(); i++){
        for (size_t j = i + 1; j < G.size(); j++){
            if (sol[i] == sol[j] and G.areConnected(G.sortedVertex(i), G.sortedVertex(j)))
                return false;
        }
    }
    return true;
}

bool no_colour_under_bound(const vector<size_t>& CV, size_t B){
    size_t c = 0;

    while (c < B){
        if (CV[c] == 0)
            return false;

        c++;
    }
    return true;
}

size_t backGCP(Graph& G, size_t k, size_t B, vector<vector<size_t>>& C, size_t vertex, vector<size_t>& S, vector<size_t>&A){
    if (vertex == G.size()){
        if (k < B){ // New best solution
            S = A;
        }
        return k;
    }

    for(size_t i = vertex; i < G.size(); i++)
        if(no_colour_under_bound(C[i],B))
            return B;

    for (size_t c = 0; c < C[vertex].size(); c++){
        if (c < B and C[vertex][c] == 0){
            for (size_t j = vertex + 1; j < G.size(); j++){
                if(G.areConnected(vertex, j)){
					C[j][c]++;
				}
            }

            A.push_back(c);
            B = backGCP(G, max(k, c), B, C, vertex+1, S, A);
            A.pop_back();

            for (size_t j = vertex + 1; j < G.size(); j++){
                if(G.areConnected(vertex, j)){
					C[j][c]--;
				}
            }
        }
    }
    return B;
}

size_t backGCPv2(Graph& G, size_t k, size_t B, vector<vector<size_t>>& C, size_t vertex, vector<size_t>& S, vector<size_t>&A){
    if (vertex == G.size()){
        if (k < B){ // New best solution
            S = A;
        }
        return k;
    }

    for(size_t i = vertex; i < G.size(); i++)
        if(no_colour_under_bound(C[i],B))
            return B;

    for (size_t c = 0; c < C[vertex].size(); c++){
        if (c < B and C[vertex][c] == 0){
            for (size_t j = vertex + 1; j < G.size(); j++){
                if(G.areConnected(G.sortedVertex(vertex), G.sortedVertex(j))){
					C[j][c]++;
				}
            }

            A.push_back(c);
            B = backGCPv2(G, max(k, c), B, C, vertex+1, S, A);
            A.pop_back();

            for (size_t j = vertex + 1; j < G.size(); j++){
                if(G.areConnected(G.sortedVertex(vertex), G.sortedVertex(j))){
					C[j][c]--;
				}
            }
        }
    }
    return B;
}

int main(){
    size_t duration, res;
    vector<size_t> sol, curr_sol;

    cout << "ASSIGNMENT 2" << endl << "--------------------------------------------------------------------" << endl;
    cout << "FIRST APPROACH (without sorting nodes)" << endl << "--------------------------------------------------------------------" << endl;
    for (size_t n = 5; n <= 30; n+=5){
        duration = 0;
        cout << "NUMBER OF COURSES: " << n << endl; 
        sol.clear();
        curr_sol.clear();
        vector<vector<size_t>>C = vector<vector<size_t>>(n, vector<size_t>(n));
        // DATA GENERATION
        cout << "Generating data...";
        generator(n, "./files/exams" + to_string(n) + "v1.txt");
        cout << "\t\t\tDone." << endl;

        // DIMACS GRAPH FILE
        cout << "Creating DIMACS graph...";
        translator("./files/exams" + to_string(n) + "v1.txt", "./files/graph" + to_string(n) + "v1.txt");
        cout << "\t\tDone." << endl;

        Graph G("./files/graph" + to_string(n) + "v1.txt");

        // GCP
        cout << "Solving Graph Colouring Problem...";
        time_point<high_resolution_clock,nanoseconds> start = high_resolution_clock::now();
        res = backGCP(G, 0, n + 1, C, 0, sol, curr_sol);
        time_point<high_resolution_clock,nanoseconds> stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start).count();
        cout << "\tDone." << endl << endl;

        cout << "Execution Time: " << duration / 1000000.0 << "ms" << endl;
        cout << "Number of days needed: " << res + 1 << endl; // 0-indexed: {0,1,2} = 3 colours

        for (size_t course = 0; course < sol.size(); course++){
            cout << "Course " << course << ": Day " << sol[course] + 1 << endl; // Days start at 0, so we add 1 
        }

        cout << "Verification: ";
        if (check_solution(G, sol))
            cout << "Valid Solution" << endl;
        else
            cout << "Invalid Solution!!!" << endl;

        cout << "--------------------------------------------------------------------" << endl;
    }

    cout << endl << endl << "SECOND APPROACH (sorting nodes by connections)" << endl << "--------------------------------------------------------------------" << endl;
    for (size_t n = 5; n <= 30; n+=5){
        duration = 0;
        cout << "NUMBER OF COURSES: " << n << endl; 
        sol.clear();
        curr_sol.clear();
        vector<vector<size_t>>C = vector<vector<size_t>>(n, vector<size_t>(n));

        // DATA GENERATION
        cout << "Generating data...";
        generator(n, "./files/exams" + to_string(n) + "v2.txt");
        cout << "\t\t\tDone." << endl;

        // DIMACS GRAPH FILE
        cout << "Creating DIMACS graph...";
        translator("./files/exams" + to_string(n) + "v2.txt", "./files/graph" + to_string(n) + "v2.txt");
        cout << "\t\tDone." << endl;

        Graph G("./files/graph" + to_string(n) + "v2.txt");

        // GCP
        cout << "Solving Graph Colouring Problem...";
        time_point<high_resolution_clock,nanoseconds> start = high_resolution_clock::now();
        res = backGCPv2(G, 0, n + 1, C, 0, sol, curr_sol);
        time_point<high_resolution_clock,nanoseconds> stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start).count();
        cout << "\tDone." << endl << endl;

        cout << "Execution Time: " << duration / 1000000.0 << "ms" << endl;
        cout << "Number of days needed: " << res + 1 << endl; // 0-indexed: {0,1,2} = 3 colours

        vector<size_t> order = G.getSortedVertexes();
        for (size_t course = 0; course < sol.size(); course++){
            cout << "Course " << order[course] << ": Day " << sol[course]+1 << endl; // Days start at 0, so we add 1 
        }

        cout << "Verification: ";
        if (check_solution_v2(G, sol))
            cout << "Valid Solution" << endl;
        else
            cout << "Invalid Solution!!!" << endl;

        cout << "--------------------------------------------------------------------" << endl;
    }
    return 0;
}
