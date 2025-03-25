#include "InvertedIndex.h"
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <iostream>
std::mutex mt;

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs;
    freq_dictionary.clear();
    std::vector<std::thread> threads;
    threads.reserve(docs.size());
    for (size_t doc_id = 0; doc_id < docs.size(); doc_id++) {
        threads.emplace_back(&InvertedIndex::multiIndex, this, static_cast<int>(doc_id));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    /*
    for (size_t doc_id = 0; doc_id < docs.size(); doc_id++) {
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
    */
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word)) {
        return freq_dictionary[word];
    }
    else {
        return {};
    }
}

void InvertedIndex::multiIndex(int doc_id) {
   
        std::stringstream ss(docs[doc_id]);
        std::string word;
        std::map<std::string, size_t> word_counts;
        if (docs[doc_id].empty()) std::cerr << "Documet " << doc_id << " is empty!" << std::endl;
        while (ss >> word) {
            word_counts[word]++;
        }

        for (const auto& [word, count] : word_counts) {
            Entry entry = { doc_id, count };
            mt.lock();
            freq_dictionary[word].push_back(entry);
            mt.unlock();
        }
   
}