#pragma once

#include "decomp.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qVector.h"

namespace net {
// The tags of a DataStore object (a vector of nex strings) and helpers that add to them (all names
// are ours).
typedef nn::nex::qVector<nn::nex::String> TagList;

DECOMP_NOINLINE void AddTag(TagList* tags, const char* tag);                     // 0x0050E57C
DECOMP_NOINLINE void AddTag(TagList* tags, const char* prefix, const wchar_t* text); // 0x0050E5B4 (prefix + text)
DECOMP_NOINLINE void AddTag(TagList* tags, const wchar_t* tag);                   // 0x0050E608

class TagWriter
{
public:
    DECOMP_EMIT_COPY void Add(const char* tag) { AddTag(m_pTags, tag); } // 0x0050E574
    DECOMP_EMIT_COPY void Add(const char* prefix, const wchar_t* text) { AddTag(m_pTags, prefix, text); } // 0x0050E5AC
    // adds the source tags for which filter returns true
    void AddIf(bool (*filter)(const wchar_t* text)); // 0x0050CFA8

    TagList* m_pTags;              // 0x0
    const TagList* m_pSourceTags;  // 0x4 (the tags that an object had)
};
} // namespace net
