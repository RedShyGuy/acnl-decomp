#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

// the names of the items: a string table and a patricia tree from names to item ids (the class and
// function names are from the reference symbols; enum, struct and member names are ours)
class nw::snd::internal::SoundArchiveFile::StringBlockBody
{
public:
    enum Sections
    {
        SECTIONS_STRING_TABLE = 0,
        SECTIONS_PATRICIA_TREE = 1,
    };

    // a node of the tree (a leaf: the string and the item)
    struct PatriciaTreeNode
    {
        static const u16 FLAG_LEAF = 1;

        u16 flags;    // 0x00
        u16 bit;      // 0x02 the bit of the name that is tested
        u32 leftIdx;  // 0x04 (bit 0)
        u32 rightIdx; // 0x08 (bit 1)
        u32 strIdx;   // 0x0C
        u32 itemId;   // 0x10
    };
    struct PatriciaTree
    {
        u32 rootIdx;               // 0x0
        u32 nodeCount;             // 0x4
        PatriciaTreeNode nodes[1]; // 0x8
    };
    // the strings: references with sizes from the table
    struct StringTable
    {
        u32 count;                           // 0x0
        BinaryReferenceWithSize items[1];    // 0x4
    };

    // the item of a name in a section (-1: none)
    u32 GetItemIdImpl(Sections section, const char* pName) const; // 0x007401D4 | fefates:bytes [tier B]
    // the string (NULL: none)
    const char* GetString(u32 strId) const; // 0x007402BC | nintendogs:bytes [tier A]

    BinaryReference sections[2]; // 0x0
};
