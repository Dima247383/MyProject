#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include <vector>
#include <string>
#include <sstream>
#include "InvertedIndex.h"

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    //idx a reference to the InvertedIndex class is passed to the class constructor,
    //so that SearchServer can find out the frequency of words encountered in the query
    SearchServer(InvertedIndex& idx) : index(idx) {};

    //return returns a sorted list of relevant answers for the given queries
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& index;
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H