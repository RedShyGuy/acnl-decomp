#include "pead/peadStringUtil.h"

namespace pead {
// 0x00538030 (name after sead)
void pead::StringUtil::parseSign_(s32* sign, Iterator* it)
{
    *sign = 1;
    char c = **it;
    if (c == '+') {
        ++*it;
    } else if (c == '-') {
        *sign = -1;
        ++*it;
    }
}

// 0x005380D0 (name after sead)
bool pead::StringUtil::detectBase_(CardinalNumber* base, Iterator* it, const SafeStringBase<char>& str)
{
    char c = **it;
    if (static_cast<u32>(c - '0') >= 10) {
        return false;
    }
    if (c != '0') {
        *base = BASE_10;
        return true;
    }
    char next = str.at(it->mIndex + 1);
    if (next == 'b') {
        *base = BASE_2;
        it->mIndex += 2;
    } else if (next == 'x') {
        *base = BASE_16;
        it->mIndex += 2;
    } else {
        *base = BASE_8;
    }
    return true;
}

// 0x0080FE14 (name after sead)
template <>
bool pead::StringUtil::parseNonDecimal_<u64>(u64* out, Iterator& it, s32 base)
{
    const u64 max = static_cast<u64>(-1);
    bool isParsed = false;
    u64 value = 0;
    const u64 limit = max / base;
    while (true) {
        s32 digit = digitValue_(*it);
        if (digit < 0 || digit >= base) {
            break;
        }
        if (value > limit || max - static_cast<u64>(digit) < value * base) {
            return false;
        }
        value = value * base + digit;
        isParsed = true;
        ++it;
    }
    if (!isParsed) {
        return false;
    }
    if (out != nullptr) {
        *out = value;
    }
    return true;
}
} // namespace pead
