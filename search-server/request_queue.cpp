#include "request_queue.h"

using namespace std::string_literals;

RequestQueue::RequestQueue(const SearchServer& search_server) 
    : search_server_(search_server)
    , current_time_(0)
    , unfound_requests_(0)
    {
    }
    
    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
        return AddToQueue(search_server_.FindTopDocuments(raw_query, status));
    }
    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
        return AddToQueue(search_server_.FindTopDocuments(raw_query));
    }
    int RequestQueue::GetNoResultRequests() const {
        return unfound_requests_;
    }

//private

    std::vector<Document> RequestQueue::AddToQueue(const std::vector<Document>& result) {
        ++current_time_;
        if (result.empty()) {
            ++unfound_requests_;
        }
        QueryResult query_result(result.size(), current_time_);
        requests_.push_back(query_result);
        CheckAmountOfDailyRequests();
        return result;
    }

    void RequestQueue::CheckAmountOfDailyRequests() {
        if (requests_.size() > min_in_day_) {
            if(requests_.front().found_docs == 0) {
                --unfound_requests_;
            }
            requests_.pop_front();
        }
    }