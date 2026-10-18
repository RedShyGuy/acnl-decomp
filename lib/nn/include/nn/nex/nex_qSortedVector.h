#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qVector.h"

#include <utility>

namespace nn {
namespace nex {
// A map in a vector of pairs sorted by the key. The name is from the reference symbols; the member
// names and the names of the inline functions are ours.
template <typename Key, typename Value>
class qSortedVector : public RootObject
{
public:
    typedef std::pair<Key, Value> value_type;
    typedef value_type* iterator;

    // (inline)
    qSortedVector() { m_Values.reserve(2); }

    iterator begin() const { return m_Values.m_pBegin; }
    iterator end() const { return m_Values.m_pEnd; }
    u32 size() const { return m_Values.size(); }
    // (inline) the first pair whose key is not less than the key
    iterator lower_bound(const Key& key) const
    {
        iterator pFirst = m_Values.m_pBegin;
        int count = m_Values.m_pEnd - pFirst;
        while (count > 0) {
            int half = count / 2;
            iterator pMiddle = pFirst + half;
            if (pMiddle->first < key) {
                pFirst = pMiddle + 1;
                count -= half + 1;
            } else {
                count = half;
            }
        }
        return pFirst;
    }
    // (inline) the pair of the key, else end()
    iterator find(const Key& key) const
    {
        iterator it = lower_bound(key);
        if (it != end() && key < it->first) {
            return end();
        }
        return it;
    }
    // (inline) the pair goes in at its place if the key is not there yet
    std::pair<iterator, bool> insert(const value_type& value)
    {
        iterator it = lower_bound(value.first);
        if (it != end() && !(value.first < it->first)) {
            return std::pair<iterator, bool>(it, false);
        }
        return std::pair<iterator, bool>(m_Values.insert(it, value), true);
    }
    // (out of line per instance) the pair of the key is removed; false if there is none
    bool erase(const Key& key)
    {
        iterator it = find(key);
        if (it == end()) {
            return false;
        }
        if (m_Values.m_pBegin != m_Values.m_pEnd) {
            m_Values.erase(it);
        }
        return true;
    }

    u32 m_Unknown0x00;        // 0x00 (not set by the constructor)
    qVector<value_type> m_Values; // 0x04
};
} // namespace nex
} // namespace nn

namespace nn {
namespace nex {
class QueuingSocket;
} // namespace nex
} // namespace nn

namespace std {
// (out of line; nex_SocketTransport.cpp)
template <>
DECOMP_NOINLINE void vector<std::pair<u16, nn::nex::QueuingSocket*>, nn::nex::MemAllocator<std::pair<u16, nn::nex::QueuingSocket*> > >::_C_insert_aux(
    std::pair<u16, nn::nex::QueuingSocket*>* pos, const std::pair<u16, nn::nex::QueuingSocket*>& value);
} // namespace std
