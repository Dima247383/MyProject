#include "SearchServer.h"
#include <algorithm>
#include <cmath>
#include <iostream> // ��� ����������� ������

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result(queries_input.size());

    for (size_t i = 0; i < queries_input.size(); ++i) {
      //  std::cout << "������: " << queries_input[i] << std::endl; // ���������� ����� �������

        std::map<size_t, float> doc_relevance; // doc_id -> relevance
        float max_relevance = 0;

        std::stringstream ss(queries_input[i]);
        std::string word;
        std::vector<std::string> query_words;
        while (ss >> word) {
            query_words.push_back(word);
          //  std::cout << "  ����� � �������: " << word << std::endl; // ���������� ����� �����
        }

        // 3. ���������� ���� �� ���������� �������
        std::sort(query_words.begin(), query_words.end(),
            [&](const std::string& a, const std::string& b) {
                return index.GetWordCount(a).size() < index.GetWordCount(b).size();
            });

        for (const std::string& query_word : query_words) {
          //  std::cout << "  ��������� �����: " << query_word << std::endl; // ���������� ����� �����
            std::vector<Entry> word_counts = index.GetWordCount(query_word);
            for (const Entry& entry : word_counts) {
              //  std::cout << "    �������� " << entry.doc_id << ": count=" << entry.count << std::endl; // ���������� �����
                doc_relevance[entry.doc_id] += entry.count; // ������� ���������� �������������
            }
        }

        // ������� ������������ �������������
        for (const auto& [doc_id, relevance] : doc_relevance) {
            max_relevance = std::max(max_relevance, relevance);
        }

        if (max_relevance == 0) {
          //  std::cout << "  ��� ����������� ����������" << std::endl; // ���������� �����
            continue; // No documents found for this query
        }

        // ������������ ������������� ������������� � ��������� � ���������
        for (const auto& [doc_id, relevance] : doc_relevance) {
            float relative_relevance = relevance / max_relevance;
          //  std::cout << "  �������� " << doc_id << ": ������������� �������������=" << relative_relevance << std::endl; // ���������� �����
            result[i].push_back({ doc_id, relative_relevance });
        }

        // ��������� ���������� �� ������������� (�� ��������)
        std::sort(result[i].begin(), result[i].end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            return a.rank > b.rank;
            });

        // ������������ ���������� ����������� (���� �����)
        // int max_responses = converter.GetResponsesLimit(); // ����� ����� �������� converter
        // if (result[i].size() > max_responses) {
        //     result[i].resize(max_responses);
        // }
    }

    return result;
}