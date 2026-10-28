#include "sead/seadPrintFormatter.h"
#include "sead/seadPrintOutput.h"
#include "sead/seadFixedSafeString.h"

#include <cstring>

namespace sead {
// 0x00545438 (name is ours)
void sead::PrintFormatter::outString_(const char* mark, PrintOutput* output, const char* str, s32 length)
{
    if (mark == nullptr || mark[1] == 's') {
        if (length == -1) {
            length = std::strlen(str);
        }
        output->write(str, length);
        return;
    }
    FixedSafeString<128> buffer;
    length = buffer.format(mark, str);
    output->write(buffer.getCString(), length);
}

// 0x00545508 | nintendogs:bytes [tier B]
bool sead::PrintFormatter::proceedToFormatMark_(char* mark)
{
    mark[0] = '\0';
    if (mFormat == nullptr) {
        return false;
    }
    if (mIsPlain) {
        return true;
    }
    if (mPosition >= mFormatLength) {
        return false;
    }
    const char* text = mFormat + mPosition;
    s32 length = 0;
    if (text[0] == '\0') {
        return false;
    }
    do {
        if (text[length] == '%') {
            if (text[length + 1] == '%') {
                // the text with one of the two percent signs
                length++;
                mOutput->write(text, length);
                mPosition += length + 1;
                text += length + 1;
                length = 0;
                continue;
            }
            if (length > 0) {
                mOutput->write(text, length);
                mPosition += length;
                text += length;
                length = 0;
            }
            s32 i = length + 1;
            s32 markLength;
            if (text[i] == '@') {
                markLength = i + 1;
            } else if (text[i] == '<') {
                mIsPlain = true;
                markLength = i + 1;
            } else {
                mark[0] = '%';
                for (;;) {
                    char c = text[i];
                    if (c == '+' || c == '-' || c == ' ' || c == '#' || c == 'h' || c == 'l' || c == 'L' || c == '.' ||
                        (c >= '0' && c <= '9')) {
                        if (i >= MARK_LENGTH_MAX - 1) {
                            break;
                        }
                        mark[i] = c;
                        i++;
                        continue;
                    }
                    if (text[i] == '\0') {
                        break;
                    }
                    // the conversion ends the mark
                    mark[i] = c;
                    markLength = i + 1;
                    mark[markLength] = '\0';
                    mPosition += markLength;
                    return true;
                }
                // a mark that is too long or not finished: the rest of the format is skipped
                mark[0] = '\0';
                mPosition = mFormatLength;
                return true;
            }
            mPosition += markLength;
            return true;
        }
        length++;
    } while (text[length] != '\0');
    if (length > 0) {
        mOutput->write(text, length);
        mPosition += length;
    }
    return false;
}

// 0x00545A00 | nintendogs:bytes [tier B]
void sead::PrintFormatter::flush()
{
    if (mFormat == nullptr) {
        return;
    }
    mIsPlain = false;
    char mark[MARK_LENGTH_MAX];
    while (mFormatLength > mPosition) {
        proceedToFormatMark_(mark);
    }
}

// 0x00545A50 (name after out)
void sead::PrintFormatter::out(const SafeStringBase<char>& str, const char* mark, PrintOutput* output)
{
    s32 length = str.getLength();
    outString_(mark, output, str.getCString(), length);
}

// 0x00545AD4 (name is ours)
sead::PrintFormatter& sead::PrintFormatter::append(const char* str)
{
    if (mFormat == nullptr) {
        mFormat = str;
        mFormatLength = std::strlen(str);
    } else {
        char mark[MARK_LENGTH_MAX];
        if (proceedToFormatMark_(mark)) {
            outString_(mark[0] != '\0' ? mark : nullptr, mOutput, str, -1);
        }
    }
    return *this;
}

template <typename T>
void sead::PrintFormatter::out(const T& value, const char* mark, PrintOutput* output)
{
    FixedSafeString<32> buffer;
    s32 length;
    if (mark != nullptr) {
        length = buffer.format(mark, value);
    } else {
        // the default mark by the signedness of T
        length = buffer.format(static_cast<T>(-1) < 0 ? "%d" : "%u", value);
    }
    output->write(buffer.getCString(), length);
}

// 0x005456E8 (name after out<int>)
template void sead::PrintFormatter::out<s8>(const s8& value, const char* mark, PrintOutput* output);
// 0x0054576C (name after out<int>)
template void sead::PrintFormatter::out<u8>(const u8& value, const char* mark, PrintOutput* output);
// 0x005457F0 | nintendogs:bytes
template void sead::PrintFormatter::out<int>(const int& value, const char* mark, PrintOutput* output);
// 0x00545874 (name after out<int>)
template void sead::PrintFormatter::out<unsigned int>(const unsigned int& value, const char* mark, PrintOutput* output);
// 0x005458F8 (name after out<int>)
template void sead::PrintFormatter::out<s16>(const s16& value, const char* mark, PrintOutput* output);
// 0x0054597C (name after out<int>)
template void sead::PrintFormatter::out<u16>(const u16& value, const char* mark, PrintOutput* output);
} // namespace sead
