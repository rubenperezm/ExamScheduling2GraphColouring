#ifndef _TRANSLATOR_H
#define _TRANSLATOR_H

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

void translator(string pathInput, string pathOutput){
    string line;
	ifstream f(pathInput);
    ofstream f2(pathOutput);
    unordered_map<string, unordered_set<string>> elems;

    if (f.is_open()){
        if (f2.is_open()){
            size_t pos = 0, n_nodes = 0, n_edges = 0;
            unordered_set<string> conflicts;
            string student, course, edges = "";

            while(getline(f,line)){
                // New course -> One node
                n_nodes += 1;

                conflicts.clear();

                // Tag course
                pos = line.find(" ");
                course = line.substr(0, pos);
                line.erase(0, pos + 1);

                while ((pos = line.find(" ")) != string::npos) {
                    // Read student
                    student = line.substr(0, pos);
                    
                    for (auto i: elems){
                        // If this student is in other course
                        if (i.second.find(student) != i.second.end()){
                            // New conflict
                            conflicts.insert(i.first);
                        }
                    }
                    // Save student
                    elems[course].insert(student);

                    // Delete student of the line
                    line.erase(0, pos + 1);
                }

                // Create edges
                for (auto i: conflicts){
                    edges += "e " + i + " " + course + "\n";
                    n_edges += 1;
                }
            }
            // File
            f2 << "c Source: " << pathInput.substr(2, pathInput.length()) << endl; // path from root directory
            f2 << "p edge " << n_nodes << " " << n_edges << endl;
            f2 << edges;

            f2.close();
        }else{
            cout << "Could not open " << pathOutput;
        }
        f.close();
    }else{
        cout << "Could not open " << pathInput;
    }
}

#endif