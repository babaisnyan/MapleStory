#pragma once

#include <array>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "allocator.h"
#include "types.h"

template <typename T, uint32_t N>
using Array = std::array<T, N>;

template <typename T>
using Vector = std::vector<T, StlAllocator<T>>;

template <typename T>
using List = std::list<T, StlAllocator<T>>;

template <typename K, typename V, typename Pr = std::less<K>>
using Map = std::map<K, V, Pr, StlAllocator<std::pair<const K, V>>>;

template <typename K, typename Pr = std::less<K>>
using Set = std::set<K, Pr, StlAllocator<K>>;

template <typename T>
using Deque = std::deque<T, StlAllocator<T>>;

template <typename T, typename Container = Deque<T>>
using Queue = std::queue<T, Container>;

template <typename T, typename Container = Deque<T>>
using Stack = std::stack<T, Container>;

template <typename T, typename Container = Vector<T>, typename Pr = std::less<typename Container::value_type>>
using PriorityQueue = std::priority_queue<T, Container, Pr>;

// using String = std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t>>;
// using StringView = std::basic_string_view<wchar_t>;

template <typename K, typename V, typename Hash = std::hash<K>, typename KeyEq = std::equal_to<K>>
using HashMap = std::unordered_map<K, V, Hash, KeyEq, StlAllocator<std::pair<const K, V>>>;

template <typename K, typename Hash = std::hash<K>, typename KeyEq = std::equal_to<K>>
using HashSet = std::unordered_set<K, Hash, KeyEq, StlAllocator<K>>;
