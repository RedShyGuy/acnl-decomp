#pragma once

#include "decomp.h"
#include "nn/nex/nex_MemAllocator.h"
#include "nn/nex/nex_MemoryManager.h"
#include <bits/stl_function.h>
#include <new>
#include <utility>

// The map of the RW STL as nex instantiates it (std::map with nex::MemAllocator; nex::qMap derives
// from it): a red-black tree whose nodes come from buffers of nodes (and a list of free nodes),
// with a header node (its parent is the root, left/right the first/last node). The class and
// template names are from the binary (__rw::__rb_tree, __rw::__rw_tree_iter,
// __rw::__rw_rb_tree_node, __rw::__select1st); ARMCC inlines the small functions, the others are
// out of line per instance and only declared here. The member names are ours.
namespace __rw {
template <typename Pair, typename Key>
struct __select1st
{
    const Key& operator()(const Pair& pair) const { return pair.first; }
};

template <typename Alloc, typename Value, typename Key, typename KeyOfValue>
struct __rw_rb_tree_node
{
    u8 m_Color;                    // 0x00
    __rw_rb_tree_node* m_pParent;  // 0x04 (the root in the header node)
    __rw_rb_tree_node* m_pLeft;    // 0x08 (the first node in the header node)
    __rw_rb_tree_node* m_pRight;   // 0x0C (the last node in the header node)
    Value m_Value;                 // 0x10
};

template <typename Value, typename Difference, typename Pointer, typename Reference, typename Node>
class __rw_tree_iter
{
public:
    __rw_tree_iter(Node* pNode) : m_pNode(pNode) {}
    __rw_tree_iter(const __rw_tree_iter& other) : m_pNode(other.m_pNode) {}
    Reference operator*() const { return m_pNode->m_Value; }
    Pointer operator->() const { return &m_pNode->m_Value; }
    bool operator==(const __rw_tree_iter& other) const { return m_pNode == other.m_pNode; }
    bool operator!=(const __rw_tree_iter& other) const { return m_pNode != other.m_pNode; }
    // the next node in order (the header's left/right trick of the RW tree)
    __rw_tree_iter& operator++()
    {
        Node* pNode = m_pNode;
        if (pNode->m_pRight != nullptr) {
            pNode = pNode->m_pRight;
            while (pNode->m_pLeft != nullptr) {
                pNode = pNode->m_pLeft;
            }
        } else {
            Node* pParent = pNode->m_pParent;
            while (pNode == pParent->m_pRight) {
                pNode = pParent;
                pParent = pParent->m_pParent;
            }
            if (pNode->m_pRight != pParent) {
                pNode = pParent;
            }
        }
        m_pNode = pNode;
        return *this;
    }

    Node* m_pNode;
};

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
class __rb_tree
{
public:
    typedef __rw_rb_tree_node<Alloc, Value, Key, KeyOfValue> Node;
    typedef __rw_tree_iter<Value, int, Value*, Value&, Node> iterator;

    // a buffer of nodes
    struct Buffer
    {
        Buffer* m_pNext; // 0x0
        u32 m_Size;      // 0x4
        Node* m_pNodes;  // 0x8
    };

    __rb_tree()
        : m_pBufferList(nullptr), m_pFreeList(nullptr), m_pNextAvail(nullptr), m_pLast(nullptr), m_pHeader(nullptr), m_Size(0),
          m_IsInsertAlways(false)
    {
        AddBuffer();
        Node* pHeader = m_pNextAvail++;
        pHeader->m_pLeft = nullptr;
        pHeader->m_pRight = nullptr;
        pHeader->m_Color = 0;
        m_pHeader = pHeader;
        pHeader->m_pParent = nullptr;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
    // (inline everywhere in the original)
    DECOMP_ALWAYS_INLINE ~__rb_tree()
    {
        if (m_pHeader != nullptr) {
            erase(begin(), end());
            m_pHeader->m_pRight = m_pFreeList;
            m_pFreeList = m_pHeader;
            while (m_pBufferList != nullptr) {
                Buffer* pBuffer = m_pBufferList;
                m_pBufferList = pBuffer->m_pNext;
                nn::nex::MemoryManager::FreeInline(pBuffer->m_pNodes);
                nn::nex::MemoryManager::FreeInline(pBuffer);
            }
        }
    }

    iterator begin() const { return iterator(m_pHeader->m_pLeft); }
    iterator end() const { return iterator(m_pHeader); }
    u32 size() const { return m_Size; }

    // the first node whose key is not less than key
    iterator lower_bound(const Key& key) const
    {
        Node* pResult = m_pHeader;
        for (Node* pNode = m_pHeader->m_pParent; pNode != nullptr;) {
            if (!Compare()(KeyOfValue()(pNode->m_Value), key)) {
                pResult = pNode;
                pNode = pNode->m_pLeft;
            } else {
                pNode = pNode->m_pRight;
            }
        }
        return iterator(pResult);
    }
    iterator find(const Key& key) const
    {
        iterator it = lower_bound(key);
        if (it == end() || Compare()(key, KeyOfValue()(it.m_pNode->m_Value))) {
            return end();
        }
        return it;
    }


    // the first node whose key is greater than key
    iterator upper_bound(const Key& key) const
    {
        Node* pResult = m_pHeader;
        for (Node* pNode = m_pHeader->m_pParent; pNode != nullptr;) {
            if (Compare()(key, KeyOfValue()(pNode->m_Value))) {
                pResult = pNode;
                pNode = pNode->m_pLeft;
            } else {
                pNode = pNode->m_pRight;
            }
        }
        return iterator(pResult);
    }
    std::pair<iterator, iterator> equal_range(const Key& key) const { return std::pair<iterator, iterator>(lower_bound(key), upper_bound(key)); }

    // the previous node in order (inline; the header's previous is the last node)
    static Node* Prev(Node* pNode)
    {
        if (pNode->m_Color == 0 && pNode->m_pParent->m_pParent == pNode) {
            return pNode->m_pRight;
        }
        if (pNode->m_pLeft != nullptr) {
            Node* pPrev = pNode->m_pLeft;
            while (pPrev->m_pRight != nullptr) {
                pPrev = pPrev->m_pRight;
            }
            return pPrev;
        }
        Node* pParent = pNode->m_pParent;
        while (pNode == pParent->m_pLeft) {
            pNode = pParent;
            pParent = pParent->m_pParent;
        }
        return pParent;
    }

    // (out of line in almost all instances; operator[] has the body inline)
    DECOMP_NOINLINE std::pair<iterator, bool> insert(const Value& value) { return InsertInline(value); }
    std::pair<iterator, bool> InsertInline(const Value& value)
    {
        Node* pParent = m_pHeader;
        Node* pNode = m_pHeader->m_pParent;
        bool isLess = true;
        while (pNode != nullptr) {
            pParent = pNode;
            isLess = Compare()(KeyOfValue()(value), KeyOfValue()(pNode->m_Value));
            pNode = isLess ? pNode->m_pLeft : pNode->m_pRight;
        }
        if (m_IsInsertAlways) {
            return std::pair<iterator, bool>(_C_insert(pNode, pParent, value), true);
        }
        iterator it(pParent);
        if (isLess) {
            if (it == begin()) {
                return std::pair<iterator, bool>(_C_insert(pNode, pParent, value), true);
            }
            it = iterator(Prev(it.m_pNode));
        }
        if (Compare()(KeyOfValue()(it.m_pNode->m_Value), KeyOfValue()(value))) {
            return std::pair<iterator, bool>(_C_insert(pNode, pParent, value), true);
        }
        return std::pair<iterator, bool>(it, false);
    }

    // (out of line per instance; not decompiled)
    // links a new node with the value under pParent (pNode: where it goes)
    iterator _C_insert(Node* pNode, Node* pParent, const Value& value);
    iterator erase(iterator it);
    // frees the subtree of the node
    void _C_erase(Node* pNode);
    // copies the subtree of the node under pParent; the copy
    Node* _C_copy(Node* pNode, Node* pParent);

    // (inline) the tree becomes a copy of other
    __rb_tree& operator=(const __rb_tree& other)
    {
        if (this != &other) {
            erase(begin(), end());
            m_pHeader->m_pParent = _C_copy(other.m_pHeader->m_pParent, m_pHeader);
            if (m_pHeader->m_pParent == nullptr) {
                m_pHeader->m_pLeft = m_pHeader;
                m_pHeader->m_pRight = m_pHeader;
            } else {
                Node* pNode = m_pHeader->m_pParent;
                while (pNode->m_pLeft != nullptr) {
                    pNode = pNode->m_pLeft;
                }
                m_pHeader->m_pLeft = pNode;
                pNode = m_pHeader->m_pParent;
                while (pNode->m_pRight != nullptr) {
                    pNode = pNode->m_pRight;
                }
                m_pHeader->m_pRight = pNode;
            }
            m_Size = other.m_Size;
        }
        return *this;
    }

    // (out of line in some instances, inline in others: the instances that the original calls out of
    // line specialize erase with the body of EraseRangeInline)
    iterator erase(iterator first, iterator last) { return EraseRangeInline(first, last); }
    iterator EraseRangeInline(iterator first, iterator last)
    {
        if (first == begin() && last == end() && m_Size != 0) {
            Node* pNode = m_pHeader->m_pParent;
            while (pNode != nullptr) {
                _C_erase(pNode->m_pRight);
                Node* pLeft = pNode->m_pLeft;
                pNode->m_Value.~Value();
                pNode->m_pRight = m_pFreeList;
                m_pFreeList = pNode;
                pNode = pLeft;
            }
            m_pHeader->m_pLeft = m_pHeader;
            m_pHeader->m_pParent = nullptr;
            m_pHeader->m_pRight = m_pHeader;
            m_Size = 0;
        } else {
            while (first != last) {
                iterator it = first;
                ++first;
                erase(it);
            }
        }
        return last;
    }

    // the first buffer: 32 nodes
    void AddBuffer()
    {
        u32 num = 32;
        Buffer* pBuffer = static_cast<Buffer*>(nn::nex::MemoryManager::Allocate(sizeof(Buffer)));
        pBuffer->m_pNodes = static_cast<Node*>(nn::nex::MemoryManager::Allocate(num * sizeof(Node)));
        pBuffer->m_pNext = m_pBufferList;
        pBuffer->m_Size = num;
        m_pBufferList = pBuffer;
        m_pNextAvail = pBuffer->m_pNodes;
        m_pLast = pBuffer->m_pNodes + num;
    }

    Buffer* m_pBufferList;  // 0x00
    Node* m_pFreeList;      // 0x04
    Node* m_pNextAvail;     // 0x08
    Node* m_pLast;          // 0x0C
    Node* m_pHeader;        // 0x10
    u32 m_Size;             // 0x14
    bool m_IsInsertAlways;  // 0x18 (multimap)
    Compare m_Compare;      // 0x19
};
} // namespace __rw

namespace std {
template <typename Key, typename T, typename Compare = less<Key>, typename Alloc = nn::nex::MemAllocator<pair<const Key, T> > >
class map
{
public:
    typedef pair<const Key, T> value_type;
    typedef __rw::__rb_tree<Key, value_type, __rw::__select1st<value_type, Key>, Compare, Alloc> rep_type;
    typedef typename rep_type::iterator iterator;

    iterator begin() const { return m_Tree.begin(); }
    iterator end() const { return m_Tree.end(); }
    u32 size() const { return m_Tree.size(); }
    iterator find(const Key& key) const { return m_Tree.find(key); }
    pair<iterator, bool> insert(const value_type& value) { return m_Tree.insert(value); }
    iterator erase(iterator it) { return m_Tree.erase(it); }
    // (out of line per instance) removes the element with the key; the number removed
    u32 erase(const Key& key);
    void clear() { m_Tree.erase(m_Tree.begin(), m_Tree.end()); }
    map& operator=(const map& other)
    {
        m_Tree = other.m_Tree;
        return *this;
    }
    // (the instances with String keys have insert inline: they specialize operator[] with the body
    // of IndexInline, inline or out of line)
    T& operator[](const Key& key) { return m_Tree.insert(value_type(key, T())).first->second; }
    T& IndexInline(const Key& key) { return m_Tree.InsertInline(value_type(key, T())).first->second; }

    // (inline everywhere in the original)
    DECOMP_ALWAYS_INLINE ~map() {}

    rep_type m_Tree; // 0x00
};

// std::multimap of the RW STL: the same tree with "insert always".
template <typename Key, typename T, typename Compare = less<Key>, typename Alloc = nn::nex::MemAllocator<pair<const Key, T> > >
class multimap
{
public:
    typedef pair<const Key, T> value_type;
    typedef __rw::__rb_tree<Key, value_type, __rw::__select1st<value_type, Key>, Compare, Alloc> rep_type;
    typedef typename rep_type::iterator iterator;

    multimap() { m_Tree.m_IsInsertAlways = true; }
    // (inline everywhere in the original)
    DECOMP_ALWAYS_INLINE ~multimap() {}

    iterator begin() const { return m_Tree.begin(); }
    iterator end() const { return m_Tree.end(); }
    u32 size() const { return m_Tree.size(); }
    pair<iterator, iterator> equal_range(const Key& key) const { return m_Tree.equal_range(key); }
    iterator insert(const value_type& value) { return m_Tree.insert(value).first; }
    iterator erase(iterator it) { return m_Tree.erase(it); }
    iterator erase(iterator first, iterator last) { return m_Tree.erase(first, last); }

    rep_type m_Tree; // 0x00
};
} // namespace std

namespace nn {
namespace nex {
// std::map with MemAllocator.
template <typename Key, typename T>
class qMap : public std::map<Key, T, std::less<Key>, MemAllocator<std::pair<const Key, T> > >
{
public:
    DECOMP_ALWAYS_INLINE ~qMap() {}
};
} // namespace nex
} // namespace nn
