#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <map>
#include <memory>
#include <deque>


#define makePair(K, V) std::make_pair(K, V)


namespace coon {
    template<typename T> using List = std::list<T>;
    template<typename T> using Vector = std::vector<T>;
    template<typename T> using Stack = std::stack<T>;
    template<typename T> using Queue = std::queue<T>;
    template<typename T> using Deque = std::deque<T>;
    template<typename K, typename V> using Pair = std::pair<K, V>;
    template<typename K, typename V> using Map = std::unordered_map<K, V>;
    template<typename K, typename V> using MultiMap = std::unordered_multimap<K, V>;
    template<typename K, typename V> using OrderedMap = std::map<K, V>;
    template<typename K, typename V> using OrderedMultiMap = std::multimap<K, V>;

    template<typename T> using Pointer = std::shared_ptr<T>;
}


#endif // CONTAINERS_H
