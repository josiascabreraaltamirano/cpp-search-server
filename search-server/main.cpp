// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:
/*#include <iostream>
#include <set>
#include <string>

using namespace std;

bool Finder(int current_number, int number_to_filter){
  string cn = to_string(current_number);
  string ntf = to_string(number_to_filter);
  for(char n : cn){
    if(n == ntf[0]){
      return true;
    }
  }
  return false;
}

set<int> CounterNumbers(int min_num_range, int max_num_range, int number_to_filter){
  set<int> result;
  for(int i = min_num_range; i <= max_num_range; i++){
    if(Finder(i, number_to_filter)){
      result.insert(i);
    }
  }
  return result;
}
int main() {
  cout << CounterNumbers(1, 1000, 3).size() << endl; 
  return 0;
}*/


// Закомитьте изменения и отправьте их в свой репозиторий.


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
        for (const string& word : document_words) {
            words_indexed[word][document_id] = /*valor Term Frecuency*/
            (count(document_words.begin(), document_words.end(),
             word) / static_cast<double>(document_words.size()));
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
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    map<string, map<int, double>> words_indexed;
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

    Query ParseQuery (const string& text) const {
        Query parse_query;
        string minus_word;
        for(string& word : SplitIntoWords(text)){
            minus_word = word.substr(1);
            if(word[0] == '-' && !IsStopWord(minus_word)){
                parse_query.minus_words.insert(minus_word);
            } else {
                parse_query.plus_words.insert(word);
            }
        }
        return parse_query;
    }

    vector<Document> FindAllDocuments(const Query& query_words) const {
        vector<Document> result;
        //map de palabra con su IDF
        //primero 
        map<string, double> words_IDF;
        map<int, double> id_relevance;

        for (const auto& word : query_words.plus_words) {
            if(words_indexed.count(word) != 0){
                words_IDF[word] = log(static_cast<double>(document_count_)/
                words_indexed.at(word).size());
            }
        }  

        for (const auto&  word : query_words.plus_words) {
            if (words_indexed.count(word) != 0) {
                for (const auto [id, TF] : words_indexed.at(word)) {
                    id_relevance[id] += (TF * words_IDF[word]);
                }
            }
        }

        for (const auto& word : query_words.minus_words) {
            if (words_indexed.count(word) != 0) {
                for (const auto value : words_indexed.at(word)) {
                    if (id_relevance.count(value.first) == 0) {
                        continue;
                    } else {
                        id_relevance.erase(value.first);
                    }
                }
            }
        }

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
