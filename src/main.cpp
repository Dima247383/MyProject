#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
	ConverterJSON converter;
	// 1. Get text documents from config.json
	std::vector<std::string> docs = converter.GetTextDocuments();

	// 2. Build an inverted index
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);

	// 3. Create a search engine
	SearchServer srv(idx);

	// 4. Get requests from requests.json
	std::vector<std::string> queries = converter.GetRequests();

	// 5. Perform a search
	std::vector<std::vector<RelativeIndex>> search_results = srv.search(queries);

	// 7. Put answers in answers.json
	converter.putAnswers(search_results);
	std::cout << "Search completed. Results written to answers.json file" << std::endl;

	return 0;
}