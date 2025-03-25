#include "SearchServer.h"
#include <algorithm>
#include <cmath>
#include <iostream> // For debug output

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result(queries_input.size());

    for (size_t i = 0; i < queries_input.size(); ++i) {
      //  std::cout << "Request: " << queries_input[i] << std::endl; // For debug output

        std::map<size_t, float> doc_relevance; // doc_id -> relevance
        float max_relevance = 0;

        std::stringstream ss(queries_input[i]);
        std::string word;
        std::vector<std::string> query_words;
        while (ss >> word) {
            query_words.push_back(word);
          //  std::cout << "  Word in query: " << word << std::endl; // For debug output
        }

        // Sort words by increasing frequency
        std::sort(query_words.begin(), query_words.end(),
            [&](const std::string& a, const std::string& b) {
                return index.GetWordCount(a).size() < index.GetWordCount(b).size();
            });

        for (const std::string& query_word : query_words) {
          //  std::cout << "  Processing the word: " << query_word << std::endl; // For debug output
            std::vector<Entry> word_counts = index.GetWordCount(query_word);
            for (const Entry& entry : word_counts) {
              //  std::cout << "    Document " << entry.doc_id << ": count=" << entry.count << std::endl; // For debug output
                doc_relevance[entry.doc_id] += entry.count; // absolute relevance
            }
        }

        // maximum relevance
        for (const auto& [doc_id, relevance] : doc_relevance) {
            max_relevance = std::max(max_relevance, relevance);
        }

        if (max_relevance == 0) {
          //  std::cout << "  No relevant documents found" << std::endl; // For debug output
            continue; // No documents found for this query
        }

        // calculate the relative relevance and add it to the result
        for (const auto& [doc_id, relevance] : doc_relevance) {
            float relative_relevance = relevance / max_relevance;
          //  std::cout << "  Document " << doc_id << ": relative relevance=" << relative_relevance << std::endl; // For debug output
            result[i].push_back({ doc_id, relative_relevance });
        }

        // Sort results by relevance (descending)
        std::sort(result[i].begin(), result[i].end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            return a.rank > b.rank;
            });

        // Limit the number of results (if necessary)
        // int max_responses = converter.GetResponsesLimit(); // need to pass the converter
        // if (result[i].size() > max_responses) {
        //     result[i].resize(max_responses);
        // }
    }

    return result;
}