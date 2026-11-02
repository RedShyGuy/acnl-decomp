#pragma once

#include "decomp.h"

namespace sead {
// Instantiations found in the binary:
//   sead::TreeMapNode<nonameg3d::Char>  typeinfo 0x008D143C
//
// A node of a red-black tree map with its key (the layout from the nodes of the game: a node of 0x1C bytes with the
// key at 0x10; member and slot names are ours).
template <typename Key>
class TreeMapNode
{
public:
    // (inline)
    TreeMapNode() : mLeft(NULL), mRight(NULL), mColor(0) {}
    virtual ~TreeMapNode() {}
    virtual void erase_() = 0; // slot 0x08

    TreeMapNode* mLeft;   // 0x04
    TreeMapNode* mRight;  // 0x08
    u32 mColor;           // 0x0C RED 0 / BLACK 1
    Key mKey;             // 0x10
};

// A left-leaning red-black tree of TreeMapNodes (the insertion of the map of the game; class, member and function
// names are ours).
template <typename Key>
class TreeMapImpl
{
public:
    typedef TreeMapNode<Key> Node;

    // the colors of the nodes
    static const u32 RED = 0;
    static const u32 BLACK = 1;

    // (inline)
    TreeMapImpl() : mRoot(NULL) {}

    // the subtree h with node inserted (a node of the same key is replaced and erased); its new root
    Node* insert(Node* h, Node* node);

    // (inline)
    static bool isRed(const Node* node) { return node != NULL && (node->mColor & 1) == RED; }
    static Node* rotateLeft(Node* h)
    {
        Node* x = h->mRight;
        h->mRight = x->mLeft;
        x->mLeft = h;
        x->mColor = h->mColor;
        h->mColor = RED;
        return x;
    }
    static Node* rotateRight(Node* h)
    {
        Node* x = h->mLeft;
        h->mLeft = x->mRight;
        x->mRight = h;
        x->mColor = h->mColor;
        h->mColor = RED;
        return x;
    }
    static void flipColors(Node* h)
    {
        h->mColor ^= 1;
        h->mLeft->mColor ^= 1;
        h->mRight->mColor ^= 1;
    }

    Node* mRoot; // 0x0
};

template <typename Key>
typename TreeMapImpl<Key>::Node* TreeMapImpl<Key>::insert(Node* h, Node* node)
{
    if (h == NULL) {
        node->mColor = RED;
        node->mLeft = NULL;
        node->mRight = NULL;
        return node;
    }
    s32 cmp = node->mKey.Compare(h->mKey);
    if (cmp < 0) {
        h->mLeft = insert(h->mLeft, node);
    } else if (cmp > 0) {
        h->mRight = insert(h->mRight, node);
    } else {
        if (h != node) {
            node->mRight = h->mRight;
            node->mLeft = h->mLeft;
            node->mColor = h->mColor;
            h->erase_();
        }
        h = node;
    }
    if (isRed(h->mRight) && !isRed(h->mLeft)) {
        h = rotateLeft(h);
    }
    if (isRed(h->mLeft) && isRed(h->mLeft->mLeft)) {
        h = rotateRight(h);
    }
    if (isRed(h->mLeft) && isRed(h->mRight)) {
        flipColors(h);
    }
    return h;
}
} // namespace sead
