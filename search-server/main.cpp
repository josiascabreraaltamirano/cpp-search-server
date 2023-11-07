#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> document_words = SplitIntoWordsNoStop(document);
        const double words_indexed_count = 1.0 / document_words.size();
        for (const string& word : document_words) {
            words_indexed_[word][document_id] += words_indexed_count; /*valor Term Frecuency*/
        }
        ++document_count_;
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    map<string, map<int, double>> words_indexed_;
    set<string> stop_words_;
    int document_count_ = 0;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord (string text) const {
        bool is_minus = false;
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery (const string& text) const {
        Query parse_query;
        string minus_word;
        for(string& word : SplitIntoWords(text)){
            minus_word = word.substr(1);//este mero
            if(word[0] == '-' && !IsStopWord(minus_word)){
                parse_query.minus_words.insert(minus_word);
            } else {
                parse_query.plus_words.insert(word);
            }
        }
        return parse_query;
    }

    double CalculateWordIDF (const string& word) const {
        return log(static_cast<double>(document_count_) / words_indexed_.at(word).size());
    }

    vector<Document> FindAllDocuments(const Query& query_words) const {

        map<int, double> id_relevance;
        for (const auto& word : query_words.plus_words) {
            if(words_indexed_.count(word) == 0){
                continue;
            }
            const double word_IDF = CalculateWordIDF(word);
            for (const auto [id, TF] : words_indexed_.at(word)) {
                id_relevance[id] += (TF * word_IDF);
            }
        }

        for (const auto& word : query_words.minus_words) {
            if (words_indexed_.count(word) != 0) {
                for (const auto value : words_indexed_.at(word)) {
                    if (id_relevance.count(value.first) == 0) {
                        continue;
                    } else {
                        id_relevance.erase(value.first);
                    }
                }
            }
        }

        vector<Document> result;
        for (const auto& doc : id_relevance) {
            result.push_back({doc.first, doc.second});
        }
        return result;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
