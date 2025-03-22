#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include <vector>
#include <string>
#include <sstream>
#include "InvertedIndex.h"

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    /**
     * @param idx � ����������� ������ ��������� ������ �� ����� InvertedIndex,
     * ����� SearchServer ��� ������ ������� ���� ����������� � �������
     */
    SearchServer(InvertedIndex& idx) : index(idx) {};

    /**
     * ����� ��������� ��������� ��������
     * @param queries_input ��������� ������� ������ �� ����� requests.json
     * @return ���������� ��������������� ������ ����������� ������� ��� �������� ��������
     */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& index;
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H