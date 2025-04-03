#define PATHCONFIG "../../../resources/config.json"
#define PATHREQUESTS "../../../resources/requests.json"

#include "ConverterJSON.h"
#include <fstream>
#include <iostream>

std::vector<std::string> ConverterJSON::GetTextDocuments() {
	std::vector<std::string> documents;
	try {
		std::ifstream f(PATHCONFIG);
		if (!f.is_open()) {
			throw std::runtime_error("config file is missing!");
		}
		else {
			f >> config_;
			if (config_.find("config") == config_.end()) {
				throw std::out_of_range("config file is empty!");
			}
			else {
				if (!config_["files"].empty()) {
					std::cout << config_["config"]["name"] << std::endl;
					for (const auto& file : config_["files"]) {
						std::ifstream file_stream(file.get<std::string>());
						if (file_stream.is_open()) {
							std::string content((std::istreambuf_iterator<char>(file_stream)),
								(std::istreambuf_iterator<char>()));
							documents.push_back(content);
							file_stream.close();
						}
						else {
							std::cerr << "Error opening file: " << file.get<std::string>() << std::endl;
						}
					}
				}
				else {
					std::cerr << "No links to documents!" << std::endl;
				}
				
			}
		}		
	}
	catch (const std::runtime_error& ex) {
		std::cerr << ex.what() << std::endl;
	}
	catch (const std::out_of_range& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return documents;
}

int ConverterJSON::GetResponsesLimit() {
	try {
		std::ifstream f(PATHCONFIG);
		config_ = nlohmann::json::parse(f);

		if (config_.contains("config") && config_["config"].contains("max_responses")) {
			return config_["config"]["max_responses"].get<int>();
		}
		else {
			return 5; // Default value if not specified
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error reading or parsing config.json: " << e.what() << std::endl;
		return 5; // Default value in case of error
	}
}

std::vector<std::string> ConverterJSON::GetRequests() {
	std::vector<std::string> requests;
	try {
		std::ifstream f(PATHREQUESTS);
		requests_ = nlohmann::json::parse(f);
		if (!f.is_open()) {
			throw std::runtime_error("requests file is missing!");
		}
		else {
			if (requests_["requests"].empty()) {
				std::cerr << "no requests" << std::endl;
			}
			else {
				if (requests_.contains("requests") && requests_["requests"].is_array()) {
					for (const auto& request : requests_["requests"]) {
						requests.push_back(request.get<std::string>());
					}
				}
			}
			
		}
	}
	catch (const std::runtime_error& ex) {
		std::cerr << ex.what() << std::endl;
	}
	return requests;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<RelativeIndex>>& answers) {
	nlohmann::json j;
	j["answers"] = nlohmann::json::object();
	for (size_t i = 0; i < answers.size(); ++i) {
		std::string request_id = "request" + std::to_string(i + 1);
		j["answers"][request_id] = nlohmann::json::object();
		if (answers[i].empty()) {
			j["answers"][request_id]["result"] = false;
		}
		else {
			j["answers"][request_id]["result"] = true;
			j["answers"][request_id]["relevance"] = nlohmann::json::object();
			for (const auto& pair : answers[i]) {
				std::string docid = std::to_string(pair.doc_id);
				j["answers"][request_id]["relevance"][docid] = { {"docid", pair.doc_id}, {"rank", pair.rank} };
			}
		}
	}

	std::ofstream file("answers.json");
	file << j.dump(4);
	file.close();
}

nlohmann::json ConverterJSON::getConfig() const {
	return config_;
}