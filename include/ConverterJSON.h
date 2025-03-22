
#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class ConverterJSON {
public:
	ConverterJSON() = default;

	/**
	 * @return Возвращает список с содержимым файлов, перечисленных в config.json
	 */
	std::vector<std::string> GetTextDocuments();

	/**
	 * @return Возвращает поле max_responses для определения предельного
	 * количества ответов на один запрос
	 */
	int GetResponsesLimit();

	/**
	 * @return возвращает список запросов из файла requests.json
	 */
	std::vector<std::string> GetRequests();

	/**
	 * Положить в файл answers.json результаты поисковых запросов
	 */
	void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);

	nlohmann::json getConfig() const;

private:
	nlohmann::json config_;
	nlohmann::json requests_;
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H