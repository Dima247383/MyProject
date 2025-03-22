#include "SearchServer.h"
#include <algorithm>
#include <cmath>
#include <iostream> // Для отладочного вывода

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result(queries_input.size());

    for (size_t i = 0; i < queries_input.size(); ++i) {
      //  std::cout << "Запрос: " << queries_input[i] << std::endl; // Отладочный вывод запроса

        std::map<size_t, float> doc_relevance; // doc_id -> relevance
        float max_relevance = 0;

        std::stringstream ss(queries_input[i]);
        std::string word;
        std::vector<std::string> query_words;
        while (ss >> word) {
            query_words.push_back(word);
          //  std::cout << "  Слово в запросе: " << word << std::endl; // Отладочный вывод слова
        }

        // 3. Сортировка слов по увеличению частоты
        std::sort(query_words.begin(), query_words.end(),
            [&](const std::string& a, const std::string& b) {
                return index.GetWordCount(a).size() < index.GetWordCount(b).size();
            });

        for (const std::string& query_word : query_words) {
          //  std::cout << "  Обработка слова: " << query_word << std::endl; // Отладочный вывод слова
            std::vector<Entry> word_counts = index.GetWordCount(query_word);
            for (const Entry& entry : word_counts) {
              //  std::cout << "    Документ " << entry.doc_id << ": count=" << entry.count << std::endl; // Отладочный вывод
                doc_relevance[entry.doc_id] += entry.count; // Считаем абсолютную релевантность
            }
        }

        // Находим максимальную релевантность
        for (const auto& [doc_id, relevance] : doc_relevance) {
            max_relevance = std::max(max_relevance, relevance);
        }

        if (max_relevance == 0) {
          //  std::cout << "  Нет релевантных документов" << std::endl; // Отладочный вывод
            continue; // No documents found for this query
        }

        // Рассчитываем относительную релевантность и добавляем в результат
        for (const auto& [doc_id, relevance] : doc_relevance) {
            float relative_relevance = relevance / max_relevance;
          //  std::cout << "  Документ " << doc_id << ": относительная релевантность=" << relative_relevance << std::endl; // Отладочный вывод
            result[i].push_back({ doc_id, relative_relevance });
        }

        // Сортируем результаты по релевантности (по убыванию)
        std::sort(result[i].begin(), result[i].end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            return a.rank > b.rank;
            });

        // Ограничиваем количество результатов (если нужно)
        // int max_responses = converter.GetResponsesLimit(); // нужно будет передать converter
        // if (result[i].size() > max_responses) {
        //     result[i].resize(max_responses);
        // }
    }

    return result;
}