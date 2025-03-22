#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#include <string>
#include <vector>
#include <map>

struct Entry {
    size_t doc_id;
    size_t count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
     * �������� ��� ��������� ���� ����������, �� ������� ����� ��������� �����
     * @param texts_input ���������� ����������
     */
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    /**
     * ����� ���������� ���������� ��������� ����� word � ����������� ����
     * ����������
     * @param word �����, ������� ��������� �������� ���������� ����������
     * @return ���������� �������������� ������ � �������� ����
     */
    std::vector<Entry> GetWordCount(const std::string& word);

private:
    std::vector<std::string> docs; // ������ ����������� ����������
    std::map<std::string, std::vector<Entry>> freq_dictionary; // ��������� �������
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H