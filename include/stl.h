#ifndef STL_H
#define STL_H
#include <numeric> //std::accumulate

inline void initial_vector_with_3_mutiplies(std::vector<int>& v, size_t size)
{
    std::vector<int> vv(size);
    int n = 3;
    std::generate(vv.begin(), vv.end(), [&n]() { auto t = n; n += 3; return t; });
    v = vv;
}

inline int count_unique_above(std::vector<int> v, int n)
{
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    int count_un = std::count_if(v.begin(), v.end(), [n](int next_val) { return next_val > n; });
    return count_un;
}

inline std::string vector_to_string(std::vector<int> v, char separator)
{
    // Right fold using reverse iterators
    std::string rs = std::accumulate(std::next(v.rbegin()), v.rend(),
        std::to_string(v.back()), // start with last element
        [separator](std::string a, int b) { return std::move(a) + separator + std::to_string(b); });
    return rs;
}
#endif // STL_H