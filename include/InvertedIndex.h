#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#include <string>
#include <vector>
#include <map>

struct Entry {
    size_t doc_id;
    size_t count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    //Update or fill in the database of documents that we will use for searching
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    //The method determines the number of occurrences of the word word in the loaded document database 
    //return returns the prepared list with the frequency of words
    std::vector<Entry> GetWordCount(const std::string& word);

    //function for multithreading, for UpdateDocumentBase
    void InvertedIndex::multiIndex(int doc_id);


private:
    std::vector<std::string> docs; // list of document contents
    std::map<std::string, std::vector<Entry>> freq_dictionary; // frequency dictionary
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H