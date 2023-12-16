#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <deque>

#include "search_server.h"
#include "document.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        explicit QueryResult(size_t found_docs_size, std::uint64_t time_mark_assigned)
        : found_docs(found_docs_size)
        , time_mark(time_mark_assigned)
        {
        }
        std::uint64_t found_docs;
        std::uint64_t time_mark;
    };

    std::deque<QueryResult> requests_;
    const SearchServer& search_server_;
    const static int min_in_day_ = 1440;
    uint64_t current_time_;
    uint64_t unfound_requests_;

    std::vector<Document> AddToQueue(const std::vector<Document>& result);

    void CheckAmountOfDailyRequests();
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate){
    return AddToQueue(search_server_.FindTopDocuments(raw_query, document_predicate));
}