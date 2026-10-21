#pragma once

#include "decomp.h"
#include "nw/ut/ut_Rect.h"

namespace nw {
namespace font {
template <typename T>
class TextWriterBase;

// The state a tag processor gets for a control character. Struct and member names are ours.
template <typename T>
struct PrintContext
{
    TextWriterBase<T>* writer; // 0x00
    const T* str;              // 0x04 the string behind the control character (the processor may skip)
    f32 xOrigin;               // 0x08 the x of the line start
    f32 yOrigin;               // 0x0C
    u32 flags;                 // 0x10 (PRINT_FLAG_*)
};

// Instantiations found in the binary:
//   nw::font::TagProcessorBase<char>  typeinfo 0x008D0C00  vtable 0x00903728
//   nw::font::TagProcessorBase<wchar_t>  typeinfo 0x008D0C08  vtable 0x00903740
//
// Handles the control characters (below ' ') of a string the text writer prints: the base
// version knows tab and line feed. The names of the virtual functions, the enums and the
// helpers are ours.
template <typename T>
class TagProcessorBase
{
public:
    // what the writer does after the control character
    enum Operation
    {
        OPERATION_DEFAULT,
        OPERATION_NO_CHAR_SPACE,  // no character space before the next character
        OPERATION_CHAR_SPACE,     // a character space before the next character
        OPERATION_NEXT_LINE,
        OPERATION_END_DRAW
    };
    // the flags of PrintContext
    static const u32 PRINT_FLAG_CHAR_SPACE_NOT_NEEDED = 1 << 0;

    // (inline)
    TagProcessorBase() {}
    virtual ~TagProcessorBase(); // slot 0x00
    // the control character is processed
    virtual Operation Process(u16 code, PrintContext<T>* pContext); // slot 0x08
    // the control character is processed and its rectangle is calculated
    virtual Operation CalcRect(nw::ut::Rect* pRect, u16 code, PrintContext<T>* pContext); // slot 0x0C

private:
    // (inline)
    void ProcessTab(PrintContext<T>* pContext);
    void ProcessLinefeed(PrintContext<T>* pContext);
};
} // namespace font
} // namespace nw
