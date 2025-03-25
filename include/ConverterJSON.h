
#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "SearchServer.h"

class ConverterJSON {
public:
	ConverterJSON() = default;

	//Returns a list with the contents of the files listed in config.json
	std::vector<std::string> GetTextDocuments();

	//Returns the max_responses field to determine the maximum number of responses per request.
	int GetResponsesLimit();

	//returns a list of requests from the requests.json file
	std::vector<std::string> GetRequests();

	//Put the search results into the answers.json file
	void putAnswers(const std::vector<std::vector<RelativeIndex>>& answers);

	nlohmann::json getConfig() const;

private:
	nlohmann::json config_;
	nlohmann::json requests_;
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H