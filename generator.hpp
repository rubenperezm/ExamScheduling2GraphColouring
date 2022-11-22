#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;
void generator(size_t n, string path){
	ofstream f(path);
	if (f.is_open()){
        unordered_set<size_t> elems;
        size_t k, nStudents;
        double pk;
        srand(time(NULL));

        for (size_t i = 0; i < n; i++){
            elems.clear();

            // File: Write course ID
            f << i;

            // Calculate the number of students for the current course
            k = rand() % n + 1;
            pk = 1/ceil(k/10.0);
            nStudents = ceil(30*pk);

            // Select random students for the current course
            while (elems.size() < nStudents){
                elems.insert(rand() % 500 + 1);
            }

            // File: Write students ID
            for (auto elem : elems)
                f << " " << elem;

            f << endl;
        }	 
	f.close();
    }else{
        cout << "Error: Could not open" << path << endl;
    }	
}

#endif