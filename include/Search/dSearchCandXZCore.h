#pragma once

#include "decomp.h"
#include "Search/dSearchCandCore.h"

// the candidates of a rectangle width x height (index = z * width + x); the derived classes test x and z (names are ours)
// RTTI 16SearchCandXZCore @ 0x008CC3BC
class SearchCandXZCore : public ::SearchCandCore
{
public:
    // (inline)
    SearchCandXZCore(s32 width, s32 height, u32* bits) : SearchCandCore(width * height, bits)
    {
        m_Width = width;
        m_Height = height;
    }
    SearchCandXZCore(); // (placeholder for the derived classes that are not decompiled yet)

    bool IsCandidate(s32 index) override; // 0x002BCEC0 slot 0x00
    virtual bool IsCandidateXZ(s32 x, s32 z) = 0; // slot 0x04

    // the candidates on the edges of the rectangle are removed
    void RemoveEdges(); // 0x00306ED0

    s32 m_Width;  // 0x14
    s32 m_Height; // 0x18
};
ASSERT_SIZE(SearchCandXZCore, 0x1C);
