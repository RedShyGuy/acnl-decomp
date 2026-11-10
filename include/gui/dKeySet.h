#pragma once

#include "decomp.h"

namespace lyt {
class ObjectEx;
}
namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
}
} // namespace ma
} // namespace ssys

namespace gui {
class Key;
class Widget;

// A keyboard layout (key set) of the software keyboard: its keys (the labels per key set index) and the buttons of a
// layout. Only the class name is from the binary (RTTI); member and function names are ours.
// RTTI N3gui6KeySetE @ 0x008D0E98
// vtable 0x00904070 (vptr 0x00904078), offset_to_top 0, 9 entries
class KeySet
{
public:
    static const s32 KEY_MAX = 64;
    // the key sets of a layout (the labels of a key per index)
    static const s32 LABEL_NUM = 16;

    // the kinds of the keys of the tables (names are ours)
    enum KeyKind
    {
        KEY_NONE = -2,       // (a wrong index or kind)
        KEY_END = -1,        // the end of a table
        KEY_CHAR = 0,        // puts its character
        KEY_SPACE = 1,       // repeats
        KEY_ENTER = 2,       // a line feed
        KEY_BACKSPACE = 3,   // repeats
        KEY_SWITCH = 4,      // switches the key sets (shift, hiragana/katakana)
        KEY_5 = 5,           // (GridKeySet: a ButtonEx; KanaKeySet: dakuten, handakuten, small kana)
        KEY_6 = 6,           // (KanaKeySet: a ButtonEx)
        KEY_7 = 7,
    };
    // a key of a table
    struct KeyEntry
    {
        s32 kind;               // 0x0 KeyKind
        const char* groupName;  // 0x4 the group of the button
        const char* labelName;  // 0x8 the label in swkbd.msbt (NULL: none)
    };
    // the key sets that the shift key toggles
    struct ShiftEntry
    {
        s32 label;
        s32 shiftedLabel;
    };

    // (inline)
    explicit KeySet(ssys::ma::lyt::ArcResAccReader* arcReader) : m_Label(0), m_IsHandled(false), m_pArcReader(arcReader)
    {
        for (s32 i = 0; i < KEY_MAX; i++) {
            m_pKeys[i] = NULL;
        }
    }
    virtual ~KeySet(); // 0x004FE788 slot 0x00
    // 0x004FE778 slot 0x04 (deleting dtor)
    // the layout and the keys are built, then the label index is set
    virtual bool Initialize(s32 label) = 0; // slot 0x08
    virtual void Update(); // 0x004FE6FC slot 0x0C
    // the event of a button (m_IsHandled: the event did something)
    virtual void OnEvent(Widget* sender, s32 event) = 0; // slot 0x10
    virtual void SetVisible(bool visible) = 0; // slot 0x14
    virtual bool IsVisible() = 0; // slot 0x18
    // the labels of the keys show the key set of the index
    virtual void SetLabel(s32 label); // 0x004FE734 slot 0x1C
    virtual lyt::ObjectEx* GetLayout() = 0; // slot 0x20

    Key* m_pKeys[KEY_MAX];                      // 0x004
    s32 m_Label;                                // 0x104
    bool m_IsHandled;                           // 0x108
    ssys::ma::lyt::ArcResAccReader* m_pArcReader; // 0x10C
};
ASSERT_SIZE(KeySet, 0x110);

// the characters the kana conversion keys put (dakuten, handakuten, small kana; sys::TextManager converts the kana
// before the cursor with them)
extern const u16 g_KanaConvertChars[3];

namespace detail {
// (inline) the kind of a key of a table: KEY_END up to ALIAS - 1 as they are, ALIAS as TARGET, others KEY_NONE
template <s32 ALIAS, s32 TARGET>
inline s32 GetKeyKind(const KeySet::KeyEntry* keys, s32 keyNum, s32 idx)
{
    if (idx < 0 || idx >= keyNum) {
        return KeySet::KEY_NONE;
    }
    const s32 kind = keys[idx].kind;
    if (kind >= KeySet::KEY_END && kind < ALIAS) {
        return kind;
    }
    if (kind == ALIAS) {
        return TARGET;
    }
    return KeySet::KEY_NONE;
}
} // namespace detail
} // namespace gui
