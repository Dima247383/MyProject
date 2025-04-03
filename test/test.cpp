#include "gtest/gtest.h"
#include "ConverterJSON.h" // Включаем заголовочный файл из основного проекта
#include "InvertedIndex.h"
#include "SearchServer.h"

// InvertedIndex
TEST(TestCase_InvertedIndex, Basic1) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const std::vector<std::string> requests = { "london", "the" };
    const std::vector<std::vector<Entry>> expected = {
            {{0, 1}},
            {{0, 1}, {1, 3}}
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    for (int i = 0; i < requests.size(); ++i)
    {
        std::vector<Entry> word_count = idx.GetWordCount(requests[i]);
        ASSERT_EQ(word_count, expected[i]);
    }
}
TEST(TestCase_InvertedIndex, Basic2) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuchino"
    };
    const std::vector<std::string> requests = { "milk", "water", "cappuchino" };
    const std::vector<std::vector<Entry>> expected = {
        { {0, 4}, {1, 1}, {2, 5} }, 
        { {0, 3}, {1, 2}, {2, 5} }, 
        { {3, 1} }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    for (int i = 0; i < requests.size(); ++i)
    {
        std::vector<Entry> word_count = idx.GetWordCount(requests[i]);
        ASSERT_EQ(word_count, expected[i]) << "requests: " << requests[i];
    }
}
TEST(TestCase_InvertedIndex, Basic3) {
    const std::vector<std::string> docs = {
            "a  b  c  d  e  f  g  h  i  j  k  l",
            "statement"
    };
    const std::vector<std::string> requests = { "m", "statement" };
    const std::vector<std::vector<Entry>> expected = {
        { },
        { {1, 1} }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    for (int i = 0; i < requests.size(); ++i)
    {
        std::vector<Entry> word_count = idx.GetWordCount(requests[i]);
        ASSERT_EQ(word_count, expected[i]);
    }
}


// ConverterJSON (проверяем, что читается конфиг)
TEST(TestCase_ConverterJSON, Limit) {
    ConverterJSON converter;
    int limit = converter.GetResponsesLimit();
    ASSERT_EQ(limit, 5); // Предполагаем, что в config.json указано max_responses = 5
}

// Test for SearchServer
TEST(TestCase_SearchServer, TestSimple) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> request = { "milk water", "sugar" };
    const std::vector<std::vector<RelativeIndex>> expected = {
        { {2, 1}, {0, 0.7}, {1, 0.3} },
        { }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCase_SearchServer, TestTop5) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };
    const std::vector<std::string> request = { "moscow is the capital of russia" };
    const std::vector<std::vector<RelativeIndex>> expected = {
        { {7, 1},{14, 1},{0, 0.666666687},{1, 0.666666687},{2, 0.666666687},
          {3, 0.666666687}, {4, 0.666666687}, {5, 0.666666687}, {6, 0.666666687},
          {8, 0.666666687}, {9, 0.666666687}, {10, 0.666666687}, {11, 0.666666687},
          {12, 0.666666687}, {13, 0.666666687}, {15, 0.666666687}, {16, 0.666666687},
          {17, 0.666666687}, {18, 0.666666687}, {19, 0.666666687}, {20, 0.666666687},
          {21, 0.666666687}
        }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result = srv.search(request);
    ASSERT_EQ(result, expected);
}


