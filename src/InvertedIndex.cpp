#include "InvertedIndex.h"
#include <sstream>
#include <algorithm>

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        std::stringstream ss(docs[doc_id]);
        std::string word;
        std::map<std::string, size_t> word_counts;

        while (ss >> word) {
            word_counts[word]++;
        }

        for (const auto& [word, count] : word_counts) {
            Entry entry = { doc_id, count };
            freq_dictionary[word].push_back(entry);
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word)) {
        return freq_dictionary[word];
    }
    else {
        return {};
    }
}
