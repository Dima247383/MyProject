#include "ConverterJSON.h"
#include <fstream>
#include <iostream>

std::vector<std::string> ConverterJSON::GetTextDocuments() {
	std::vector<std::string> documents;
	try {
		std::ifstream f("E://cmakeprojects//exemple1//resources//config.json"); 
		config_ = nlohmann::json::parse(f);

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
	catch (const std::exception& e) {
		std::cerr << "Error reading or parsing config.json: " << e.what() << std::endl;
	}
	return documents;
}

int ConverterJSON::GetResponsesLimit() {
	try {
		std::ifstream f("E://cmakeprojects//exemple1//resources//config.json");
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
		std::ifstream f("E://cmakeprojects//exemple1//resources//requests.json");
		requests_ = nlohmann::json::parse(f);

		if (requests_.contains("requests") && requests_["requests"].is_array()) {
			for (const auto& request : requests_["requests"]) {
				requests.push_back(request.get<std::string>());
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error reading or parsing requests.json: " << e.what() << std::endl;
	}
	return requests;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
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
				std::string docid = std::to_string(pair.first);
				j["answers"][request_id]["relevance"][docid] = { {"docid", pair.first}, {"rank", pair.second} };
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