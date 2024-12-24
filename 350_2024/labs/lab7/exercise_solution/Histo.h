#ifndef HISTO_H_
#define HISTO_H_

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Histo {
public:
    // update word count
    void count(const std::string &w) {
        // creates (w, int) pair if not present (initialized with count 0), and
        // then increments int (map[key] returns reference to the second pair
        // component (data associated with key))

        ++string2int[w];

#if 0
    // the effect of above statement is
    auto it = string2int.find(w);
    if (it == end(string2int)) {
      string2int.insert({w, 1}); // w not there - add it with count 1
    } else {
      it->second++;              // increase count
    }
#endif
    }

    // print histogram in non-increasing count order to stream os
    void print(std::ostream &os) const {
        std::vector<Pair> v;

        // create vector of (word, int) pairs from map
        // needed because std::map can't be sorted as
        // element location depends on keys
        // Note that this will actually use a const_iterator
        // because print is a const function
        for (auto &p : string2int) {
            v.push_back({p.first, p.second});
        }

        // sort pairs by count and word (*)
        std::sort(begin(v), end(v), PairComp());

        // print sorted list
        for (auto &p : v) {
            os << p.word << " " << p.count << std::endl;
        }
    }

private:
    // data members and helper functions go here

    // for sorting list
    struct Pair {
        std::string word;
        int count;
    };

    // lexicographic ordering: higher count first, then smaller word
    struct PairComp {
        // return true iff a "<" b
        // sorting will call comp_object(a, b) to compare a and b in line (*)
        bool operator()(const Pair &a, const Pair &b) {
            return a.count > b.count || (a.count == b.count && a.word < b.word);
        }
    };

    // maps words to their count
    std::map<std::string, int> string2int;
};

#endif    // HISTO_H_
