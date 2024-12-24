#ifndef HISTO_H_
#define HISTO_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

class Histo {
public:
    // update word count
    void count(const std::string &w) {
        // Increment the count for word 'w' in the map
        ++wordCounts[w];
    }

    // print histogram in non-increasing count order to stream os
    void print(std::ostream &os) const {
        // Create a vector of pairs from the map for sorting
        std::vector<std::pair<std::string, int>> vec(wordCounts.begin(), wordCounts.end());

        // Sort the vector by count in descending order
        for (auto &p : vec) {
            std::transform(p.first.begin(), p.first.end(), p.first.begin(), ::tolower);
        }
        

        // Iterate through the sorted vector and print each word with its count
        for(const auto &p : vec){
            os << p.first << " " << p.second << "\n";
        }
    }

private:
    // Map to store word counts
    std::map<std::string, int> wordCounts;
};

#endif    // HISTO_H_