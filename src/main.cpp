#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
	setlocale(LC_ALL, "");
	ConverterJSON converter;

	// 1. Получить текстовые документы из config.json
	std::vector<std::string> docs = converter.GetTextDocuments();

	// 2. Построить инвертированный индекс
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);

	// 3. Создать поисковый сервер
	SearchServer srv(idx);

	// 4. Получить запросы из requests.json
	std::vector<std::string> queries = converter.GetRequests();

	// 5. Выполнить поиск
	std::vector<std::vector<RelativeIndex>> search_results = srv.search(queries);

	// 6. Преобразовать search_results в нужный тип
	std::vector<std::vector<std::pair<int, float>>> answers_for_json;
	for (const auto& query_results : search_results) {
		std::vector<std::pair<int, float>> query_answers;
		for (const auto& relative_index : query_results) {
			query_answers.push_back({ static_cast<int>(relative_index.doc_id), relative_index.rank });
		}
		answers_for_json.push_back(query_answers);
	}


	// 7. Положить ответы в answers.json
	converter.putAnswers(answers_for_json);

	std::cout << "Поиск завершен. Результаты записаны в файл answers.json" << std::endl;

	return 0;
}