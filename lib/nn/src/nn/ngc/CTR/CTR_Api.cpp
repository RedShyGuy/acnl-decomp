#include "nn/ngc/CTR/CTR_Api.h"

namespace nn {
namespace ngc {
namespace CTR {
namespace {
inline bool IsInRange(wchar_t c, wchar_t first, wchar_t last)
{
    return c >= first && c <= last;
}
} // namespace

// 0x003DFEF8 | fefates:bytes [tier B]
int CountNumbers(const wchar_t* text)
{
    if (text == NULL) {
        return -1;
    }
    int count = 0;
    for (; *text != 0; text++) {
        wchar_t c = *text;
        if (IsInRange(c, L'0', L'9') || IsInRange(c, 0xFF10, 0xFF19) ||    // full width
            IsInRange(c, 0x2070, 0x2079) || IsInRange(c, 0x2080, 0x2089) || // superscripts, subscripts
            IsInRange(c, 0x2160, 0x216B) || IsInRange(c, 0x2170, 0x217B) || // Roman numerals
            IsInRange(c, 0x2460, 0x2473) || IsInRange(c, 0x2474, 0x2487) || IsInRange(c, 0x2488, 0x249B) ||   // enclosed
            IsInRange(c, 0x2776, 0x277F) || IsInRange(c, 0x2780, 0x2789) || IsInRange(c, 0x278A, 0x2793) ||   // dingbats
            c == 0xB2 || c == 0xB3 || c == 0xB9 || c == 0x24EA || c == 0x24FF) {
            count++;
        }
    }
    return count;
}

} // namespace CTR
} // namespace ngc
} // namespace nn
