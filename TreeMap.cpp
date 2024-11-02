#ifndef TREE_MAP_CPP
#define TREE_MAP_CPP

#include "TreeMap.hpp"
#include "TreeSet.cpp"
#include <functional> // for std::greater, std::less_equal

template <typename TKey, typename TValue>
TreeMap<TKey, TValue>::TreeMap() : _tree([](const std::pair<TKey, TValue>& a, const std::pair<TKey, TValue>& b) {
    return a.first < b.first ? -1 : (a.first > b.first ? 1 : 0);
}) {}

template <typename TKey, typename TValue>
TreeMap<TKey, TValue>::TreeMap(const std::vector<std::pair<TKey, TValue>> &items) : TreeMap() {
    for ( auto item : items) {
        insert(item.first, item.second);
    }
}

template <typename TKey, typename TValue>
size_t TreeMap<TKey, TValue>::size() const {
    return _tree.size();
}

template <typename TKey, typename TValue>
void TreeMap<TKey, TValue>::insert(TKey key, TValue value) {
    _tree.add({key, value});
}

template <typename TKey, typename TValue>
std::optional<TValue> TreeMap<TKey, TValue>::get(TKey key) const {
    auto check = _tree.get({key, TValue()});
    if (check) {
        return check->second;
    }
    return std::nullopt;
}

template <typename TKey, typename TValue>
bool TreeMap<TKey, TValue>::contains(TKey key) const {
    return _tree.contains({key, TValue()});
}

template <typename TKey, typename TValue>
std::vector<std::pair<TKey, TValue>> TreeMap<TKey, TValue>::to_vector() const {
    return _tree.to_vector();
}

template <typename TKey, typename TValue>
bool TreeMap<TKey, TValue>::is_empty() const {
    return _tree.is_empty();
}

template <typename TKey, typename TValue>
void TreeMap<TKey, TValue>::clear() {
    _tree.clear();
}

template <typename TKey, typename TValue>
TreeMap<TKey, TValue>::~TreeMap() {
    clear();
}
#endif
