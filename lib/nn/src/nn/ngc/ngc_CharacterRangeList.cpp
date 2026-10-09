#include "nn/ngc/ngc_CharacterRangeList.h"

namespace nn {
namespace ngc {
// 0x003DF05C | fefates:bytes [tier B]
bool nn::ngc::CharacterRangeList::RemoveFromClass(const nn::ngc::CharacterRangeList* other)
{
    Iterator it = Begin();
    ConstIterator remove = other->Begin();
    while (it != End() && remove != other->End()) {
        wchar_t begin = it->m_Begin;
        wchar_t removeBegin = remove->m_Begin;
        wchar_t end = it->m_End;
        wchar_t removeEnd = remove->m_End;
        if (begin == removeBegin) {
            if (end <= removeEnd) {
                it = Erase(it);
            } else {
                it->m_Begin = removeEnd + 1;
                ++remove;
            }
        } else if (begin > removeBegin) {
            if (begin > removeEnd) {
                ++remove;
            } else if (removeEnd >= end) {
                it = Erase(it);
            } else {
                it->m_Begin = removeEnd + 1;
            }
        } else {
            if (end < removeBegin) {
                ++it;
                continue;
            }
            it->m_End = removeBegin - 1;
            if (end <= removeEnd) {
                ++it;
                continue;
            }
            // the rest behind the removed range
            Iterator rest = Insert(Iterator(it.m_Node->m_Next));
            if (rest.IsNull()) {
                return false;
            }
            rest->m_Begin = removeEnd + 1;
            rest->m_End = end;
            ++remove;
            it = rest;
        }
    }
    return true;
}

// 0x003DF174 | fefates:bytes [tier B]
bool nn::ngc::CharacterRangeList::RemoveFromClass(wchar_t character)
{
    for (Iterator it = Begin(); it != End(); ++it) {
        wchar_t begin = it->m_Begin;
        wchar_t end = it->m_End;
        if (begin == character) {
            if (end == character) {
                Erase(it);
            } else {
                it->m_Begin = begin + 1;
            }
            return true;
        }
        if (end == character) {
            it->m_End = end - 1;
            return true;
        }
        if (begin <= character && character <= end) {
            it->m_End = character - 1;
            Iterator rest = Insert(Iterator(it.m_Node->m_Next));
            if (rest.IsNull()) {
                return false;
            }
            rest->m_Begin = character + 1;
            rest->m_End = end;
            return true;
        }
    }
    return true;
}

// 0x003DF260 | fefates:bytes [tier B]
bool nn::ngc::CharacterRangeList::AddCharacterRange(wchar_t begin, wchar_t end)
{
    bool isMerged = false;
    Iterator it = Begin();
    while (it != End()) {
        wchar_t rangeBegin = it->m_Begin;
        wchar_t rangeEnd = it->m_End;
        if (rangeBegin <= begin && end <= rangeEnd) {
            return true;
        }
        if (begin <= rangeBegin && rangeEnd <= end) {
            it = Erase(it);
            continue;
        }
        if ((rangeBegin <= begin && begin <= rangeEnd) || (rangeEnd != CHARACTER_MAX && rangeEnd + 1 == begin)) {
            // the new range goes on behind this one
            isMerged = true;
            it->m_End = end;
        } else if ((rangeBegin <= end && end <= rangeEnd) || (end != CHARACTER_MAX && end + 1 == rangeBegin)) {
            // the new range reaches into this one
            if (!isMerged) {
                it->m_Begin = begin;
                return true;
            }
            it = Erase(it);
            --it;
            it->m_End = rangeEnd;
            return true;
        } else if (end < rangeBegin) {
            if (isMerged) {
                return true;
            }
            Iterator range = Insert(it);
            if (range.IsNull()) {
                return false;
            }
            range->m_Begin = begin;
            range->m_End = end;
            return true;
        }
        ++it;
    }
    if (isMerged) {
        return true;
    }
    Iterator range = PushBackNew();
    if (range.IsNull()) {
        return false;
    }
    range->m_Begin = begin;
    range->m_End = end;
    return true;
}

// 0x003DF404 | fefates:bytes [tier B]
bool nn::ngc::CharacterRangeList::MergeCharacterRanges(const nn::ngc::CharacterRangeList* other)
{
    for (ConstIterator it = other->Begin(); it != other->End(); ++it) {
        if (!AddCharacterRange(it->m_Begin, it->m_End)) {
            return false;
        }
    }
    return true;
}

// 0x003DF44C | fefates:bytes [tier B]
bool nn::ngc::CharacterRangeList::Inverse()
{
    if (IsEmpty()) {
        Iterator range = PushBackNew();
        if (range.IsNull()) {
            return false;
        }
        range->m_Begin = 0;
        range->m_End = CHARACTER_MAX;
        return true;
    }

    s32 count = 0;
    for (Iterator it = Begin(); it != End(); ++it) {
        count++;
    }
    Iterator first = Begin();
    // (the original tests the end against 0 here)
    if (count == 1 && first->m_Begin == 0 && first->m_End == 0) {
        Erase(first);
        return true;
    }

    CharacterRangeList inverse;
    inverse.SetPool(m_Pool);
    Iterator it = Begin();
    if (it->m_Begin != 0) {
        Iterator range = inverse.PushBackNew();
        if (range.IsNull()) {
            return false;
        }
        range->m_Begin = 0;
        range->m_End = it->m_Begin - 1;
    }
    for (Iterator next(it.m_Node->m_Next); next != End(); next = Iterator(it.m_Node->m_Next)) {
        Iterator range = inverse.PushBackNew();
        if (range.IsNull()) {
            return false;
        }
        range->m_Begin = it->m_End + 1;
        range->m_End = next->m_Begin - 1;
        it = next;
    }
    if (it->m_End != CHARACTER_MAX) {
        Iterator range = inverse.PushBackNew();
        if (range.IsNull()) {
            return false;
        }
        range->m_Begin = it->m_End + 1;
        range->m_End = CHARACTER_MAX;
    }
    MoveFrom(inverse);
    return true;
}

namespace {
// the rest of the range at it behind withEnd (up to end) as a new range behind it; it is the
// new range then (name is ours)
DECOMP_ALWAYS_INLINE bool SplitBehind(CharacterRangeList* list, CharacterRangeList::Iterator& it, wchar_t withEnd, wchar_t end)
{
    CharacterRangeList::Iterator rest = list->Insert(CharacterRangeList::Iterator(it.m_Node->m_Next));
    if (rest.IsNull()) {
        return false;
    }
    rest->m_Begin = withEnd + 1;
    rest->m_End = end;
    it = rest;
    return true;
}
} // namespace

// 0x003DF7B8 | fefates:bytes [tier B]
bool nn::ngc::CharacterRangeList::Intersect(const nn::ngc::CharacterRangeList* other)
{
    Iterator it = Begin();
    ConstIterator with = other->Begin();
    while (it != End()) {
        if (with == other->End()) {
            // nothing left to intersect with
            do {
                it = Erase(it);
            } while (it != End());
            return true;
        }
        wchar_t begin = it->m_Begin;
        wchar_t withBegin = with->m_Begin;
        wchar_t end = it->m_End;
        wchar_t withEnd = with->m_End;
        if (begin == withBegin) {
            if (end == withEnd) {
                ++it;
                ++with;
                continue;
            }
            if (end < withEnd) {
                ++it;
                continue;
            }
            it->m_End = withEnd;
            if (!SplitBehind(this, it, withEnd, end)) {
                return false;
            }
            ++with;
        } else if (begin < withBegin) {
            if (end < withBegin) {
                it = Erase(it);
                continue;
            }
            it->m_Begin = withBegin;
            if (end <= withEnd) {
                ++it;
                continue;
            }
            it->m_End = withEnd;
            if (!SplitBehind(this, it, withEnd, end)) {
                return false;
            }
            ++with;
        } else {
            if (withEnd < begin) {
                ++with;
                continue;
            }
            if (withEnd >= end) {
                ++it;
                continue;
            }
            it->m_End = withEnd;
            if (!SplitBehind(this, it, withEnd, end)) {
                return false;
            }
            ++with;
        }
    }
    return true;
}

// 0x0072EBBC | fefates:bytes [tier B]
bool nn::ngc::CharacterRangeList::HasSharedCharRangeArea(const nn::ngc::CharacterRangeList* other) const
{
    ConstIterator it = Begin();
    ConstIterator with = other->Begin();
    while (it != End() && with != other->End()) {
        if (it->m_Begin <= with->m_Begin) {
            if (with->m_Begin <= it->m_End) {
                return true;
            }
            ++it;
        } else {
            if (it->m_Begin <= with->m_End) {
                return true;
            }
            ++with;
        }
    }
    return false;
}

} // namespace ngc
} // namespace nn
