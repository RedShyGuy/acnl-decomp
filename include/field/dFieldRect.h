#pragma once

#include "decomp.h"
#include "field/dFieldTypes.h"

// A rectangle of blocks or units with inclusive bounds (layout and names after libgarden field/FieldRect.hpp; the
// functions without libgarden name are ours).
struct FieldRect {
    // walks over all positions of the rectangle, row by row
    class iterator
    {
    public:
        DECOMP_EMIT_COPY iterator() : m_pRect(NULL), m_pEnd(NULL), m_Position(0, 0) {} // 0x0052F7D4
        DECOMP_EMIT_COPY ~iterator() {} // 0x0052F7EC

        // (two copies: const and not const)
        DECOMP_EMIT_COPY const Vector2i& operator*() const { return m_Position; } // 0x00748F64
        DECOMP_EMIT_COPY Vector2i& operator*() { return m_Position; } // 0x00748EFC

        iterator& operator++(); // 0x0052F7F0 | libgarden [tier A]
        friend bool operator!=(const iterator& left, const iterator& right); // 0x00748EA4 | libgarden [tier A]

    private:
        friend struct FieldRect;

        iterator(const FieldRect* rect, const Vector2i* end) : m_pRect(rect), m_pEnd(end), m_Position(0, 0) {}

        const FieldRect* m_pRect; // 0x0
        const Vector2i* m_pEnd;   // 0x4 the maximum of the rectangle
        Vector2i m_Position;      // 0x8
    };

    DECOMP_EMIT_COPY DECOMP_NOINLINE FieldRect() : min(0, 0), max(0, 0) {} // 0x0052F8F4
    DECOMP_EMIT_COPY DECOMP_NOINLINE FieldRect(const Vector2i& min_, const Vector2i& max_) // 0x0052F8AC
        : min(min_.x, min_.y), max(max_.x, max_.y)
    {
    }
    DECOMP_EMIT_COPY DECOMP_NOINLINE FieldRect(const FieldRect& other) : min(other.min.x, other.min.y), max(other.max.x, other.max.y) {} // 0x0052F8D0
    DECOMP_EMIT_COPY ~FieldRect() {} // 0x0052F92C

    DECOMP_EMIT_COPY DECOMP_NOINLINE FieldRect& operator=(const FieldRect& other) // 0x0052F930
    {
        min.x = other.min.x;
        min.y = other.min.y;
        max.x = other.max.x;
        max.y = other.max.y;
        return *this;
    }
    // moves the rectangle (name is ours)
    DECOMP_EMIT_COPY DECOMP_NOINLINE FieldRect& operator+=(const Vector2i& offset) // 0x0052F954
    {
        min.x += offset.x;
        min.y += offset.y;
        max.x += offset.x;
        max.y += offset.y;
        return *this;
    }
    // grows the rectangle so that it contains the position (name is ours)
    void Include(const Vector2i& pos); // 0x0052F858

    iterator begin() const; // 0x00748F3C | libgarden [tier A]
    iterator end() const;   // 0x00748F04 | libgarden [tier A]

    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetWidth() const { return (max.x - min.x) + 1; }  // 0x00748F74
    DECOMP_EMIT_COPY DECOMP_NOINLINE s32 GetHeight() const { return (max.y - min.y) + 1; } // 0x00748FC8
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool Contains(const Vector2i& pos) const // 0x00748F88
    {
        if (min.x <= pos.x && pos.x <= max.x && min.y <= pos.y && pos.y <= max.y) {
            return true;
        }
        return false;
    }
    // the index of a position relative to the minimum (libgarden: ToIndex)
    s32 ToIndex(s32 x, s32 z) const { return z * GetWidth() + x; }

    Vector2i min; // 0x0
    Vector2i max; // 0x8
};
ASSERT_SIZE(FieldRect, 0x10);
ASSERT_SIZE(FieldRect::iterator, 0x10);

// the rectangle moved back by the offset (name is ours)
DECOMP_EMIT_COPY DECOMP_NOINLINE inline FieldRect operator-(const FieldRect& rect, const Vector2i& offset) // 0x00748FDC
{
    FieldRect result;
    result.min.x = rect.min.x - offset.x;
    result.min.y = rect.min.y - offset.y;
    result.max.x = rect.max.x - offset.x;
    result.max.y = rect.max.y - offset.y;
    return result;
}
