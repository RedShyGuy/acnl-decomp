#pragma once

#include "decomp.h"

namespace nw {
namespace lyt {
class TextBox;
}
} // namespace nw

namespace script {
class IWord;

// The measuring of texts (the class name is from the RTTI of MeasureProcessor, the functions and their names are
// ours).
class Chip
{
public:
    class MeasureProcessor;

    // the width of the text between *begin and *end with the font of the text box
    static f32 MeasureWidth(const nw::lyt::TextBox* pTextBox, const char16* const* begin, const char16* const* end); // 0x005EAB78
    // the width of a word with the message font at scale 1
    static f32 MeasureWidth(const IWord* word); // 0x005EAAAC
    // the scale (at most 1) for the word to fit into the width of the text box
    static f32 CalcFitScale(const nw::lyt::TextBox* pTextBox, const IWord* word); // 0x005EA938
};
} // namespace script
