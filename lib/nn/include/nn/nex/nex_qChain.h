#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// The links of an element of a qChain: DefaultChainPolicy uses m_pChainNext and m_pChainPrev of
// the element (their offsets differ per class). The member names are ours.
template <typename T>
class DefaultChainPolicy
{
public:
    static T& Next(T p) { return p->m_pChainNext; }
    static T& Prev(T p) { return p->m_pChainPrev; }
};

// An intrusive doubly linked list of pointers (the links are in the elements). Instantiations in
// the binary:
//   qChain<BandwidthCounter*, DefaultChainPolicy<BandwidthCounter*> >  typeinfo 0x008CF60C  vtable 0x008FFAB8
//   qChain<Job*, DefaultChainPolicy<Job*> >  typeinfo 0x008CF624  vtable 0x008FFAD8
//   qChain<Packet*, ChainPolicyHistoryPacket<Packet*> >  typeinfo 0x008CF648  vtable 0x008FFB08
//   qChain<Packet*, ChainPolicyTemp<Packet*> >  typeinfo 0x008CF630  vtable 0x008FFAE8
//   qChain<Packet*, DefaultChainPolicy<Packet*> >  typeinfo 0x008CF63C  vtable 0x008FFAF8
//   qChain<ProfilingUnit*, DefaultChainPolicy<ProfilingUnit*> >  typeinfo 0x008CF5F4  vtable 0x008FFA98
//   qChain<SystemSetting*, DefaultChainPolicy<SystemSetting*> >  typeinfo 0x008CF600  vtable 0x008FFAA8
//   qChain<ThreadVariableRoot*, DefaultChainPolicy<ThreadVariableRoot*> >  typeinfo 0x008CF618  vtable 0x008FFAC8
// The member names are ours.
template <typename T, typename Policy>
class qChain : public ::nn::nex::RootObject
{
public:
    class iterator
    {
    public:
        iterator(T p) : m_p(p) {}
        // (a class with a copy constructor: passed and returned in memory like in the original)
        iterator(const iterator& other) : m_p(other.m_p) {}
        T operator*() const { return m_p; }
        bool operator==(const iterator& other) const { return m_p == other.m_p; }
        bool operator!=(const iterator& other) const { return m_p != other.m_p; }

        T m_p;
    };

    qChain() : m_pFirst(nullptr), m_pLast(nullptr), m_pEnd(nullptr), m_Count(0) {}
    virtual ~qChain()
    {
        iterator it = begin();
        while (it != end()) {
            it = erase(it);
        }
    }

    iterator begin() const { return iterator(m_pFirst); }
    iterator end() const { return iterator(m_pEnd); }
    u32 size() const { return m_Count; }

    void push_back(T p)
    {
        if (m_pFirst == m_pEnd) {
            m_pFirst = p;
            m_pLast = p;
        } else {
            Policy::Next(m_pLast) = p;
            Policy::Prev(p) = m_pLast;
            m_pLast = p;
        }
        m_Count++;
    }
    void push_front(T p)
    {
        if (m_pFirst == m_pEnd) {
            m_pFirst = p;
            m_pLast = p;
        } else {
            Policy::Next(p) = m_pFirst;
            Policy::Prev(m_pFirst) = p;
            m_pFirst = p;
        }
        m_Count++;
    }
    // unlinks the element; the next one (instances that the original calls out of line specialize
    // erase with the body of EraseInline)
    iterator erase(iterator it) { return EraseInline(it); }
    iterator EraseInline(iterator it)
    {
        T p = *it;
        T prev = Policy::Prev(p);
        iterator next(Policy::Next(p));
        if (prev != nullptr) {
            Policy::Next(prev) = *next;
        }
        Policy::Prev(p) = nullptr;
        if (*next != nullptr) {
            Policy::Prev(*next) = prev;
        }
        Policy::Next(p) = nullptr;
        if (m_pFirst == p) {
            m_pFirst = *next;
        }
        if (m_pLast == p) {
            m_pLast = prev;
        }
        m_Count--;
        return next;
    }
    // unlinks every occurrence of p
    void remove(T p)
    {
        iterator it = begin();
        while (it != end()) {
            while (it != end() && *it != p) {
                it = iterator(Policy::Next(*it));
            }
            if (it == end()) {
                break;
            }
            it = erase(it);
        }
    }

    T m_pFirst;  // 0x04
    T m_pLast;   // 0x08
    T m_pEnd;    // 0x0C (always null)
    u32 m_Count; // 0x10
};
} // namespace nex
} // namespace nn
