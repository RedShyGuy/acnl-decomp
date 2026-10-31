#pragma once

// pair, swap and the function objects of the C++ library ARMCC linked in (Rogue Wave). Names after the C++ standard.

namespace std {
template <typename T1, typename T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair() : first(), second() {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    template <typename U1, typename U2>
    pair(const pair<U1, U2>& other) : first(other.first), second(other.second)
    {
    }
};

template <typename T1, typename T2>
inline bool operator==(const pair<T1, T2>& a, const pair<T1, T2>& b)
{
    return a.first == b.first && a.second == b.second;
}

template <typename T1, typename T2>
inline bool operator<(const pair<T1, T2>& a, const pair<T1, T2>& b)
{
    return a.first < b.first || (!(b.first < a.first) && a.second < b.second);
}

template <typename T1, typename T2>
inline pair<T1, T2> make_pair(const T1& a, const T2& b)
{
    return pair<T1, T2>(a, b);
}

template <typename T>
inline void swap(T& a, T& b)
{
    T swapped = a;
    a = b;
    b = swapped;
}

template <typename Argument1, typename Argument2, typename Result>
struct binary_function
{
    typedef Argument1 first_argument_type;
    typedef Argument2 second_argument_type;
    typedef Result result_type;
};

template <typename T>
struct less : public binary_function<T, T, bool>
{
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename T>
struct equal_to : public binary_function<T, T, bool>
{
    bool operator()(const T& a, const T& b) const { return a == b; }
};
} // namespace std
