#pragma once

#include "decomp.h"

namespace script {
class IWord;

// The measuring of the width of messages (with their inserted words) for the text boxes; slot names are ours.
// RTTI N6script8IMeasureE @ 0x008D3DE8
class IMeasure
{
public:
    // (inline)
    IMeasure() {}
    virtual ~IMeasure() {}
    // the measure of the inserted texts (NULL: none)
    virtual IMeasure* GetChild() = 0; // slot 0x08
    // the width of the message with its inserted words
    virtual f32 Measure(const IWord* word) = 0; // slot 0x0C
    // the width of a text
    virtual f32 MeasureText(const char16* const* begin, const char16* const* end) = 0; // slot 0x10
    // (purpose unknown; false by default)
    virtual bool IsFlag13(); // 0x0075DC80 slot 0x14
};
} // namespace script
