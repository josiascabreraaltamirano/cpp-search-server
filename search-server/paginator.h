#pragma once

#include <vector>
#include <iterator>
#include <iostream>

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
    : begin_(begin),
      end_(end) {}

    auto begin() const {
        return begin_;
    }
    auto end() const {
        return end_;
    }
    int size() const {
        return std::distance(begin_, end_);
    }

private:
    Iterator begin_;
    Iterator end_; 
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        Iterator i_helper = begin;
        for (Iterator i = begin; i != end; i++) {
            if (page_size == std::distance(i_helper, i)) {
                IteratorRange page(i_helper, i);
                pages_.push_back(page);
                i_helper = i;
            }
            if (std::distance(i_helper, end) <= page_size) {
                IteratorRange last_page(i_helper, end);
                pages_.push_back(last_page);
                break;
            }
        }
    }
    auto begin() const {
        return pages_.begin();
    }
    auto end() const {
        return pages_.end();
    }
    size_t size() const {
        return pages_.size();
    }


private:
    std::vector<IteratorRange<Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& output, const IteratorRange<Iterator>& range) {
    for (auto it = range.begin(); it != range.end(); ++it) {
        output << *it;
    }
    return output;
}