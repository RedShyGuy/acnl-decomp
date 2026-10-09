#include "nn/nwm/CTR/nwm_BssReader.h"
#include <string.h>

namespace nn {
namespace nwm {
namespace CTR {
namespace {
// the information elements (IEEE 802.11)
const u8 ELEMENT_ID_SSID = 0;
const u8 ELEMENT_ID_VENDOR_SPECIFIC = 0xDD;
const size_t OUI_SIZE = 3;

// an element: tag, length and data
inline const u8* NextElement(const u8* element)
{
    return element + 2 + element[1];
}
} // namespace

// 0x0072ECF4 (name is ours)
u8 nn::nwm::CTR::BssReader::GetIeNum() const
{
    const u8* element = GetIePointer();
    if (element == NULL) {
        return 0;
    }
    const u8* end = element + GetIeSize();
    u8 num = 0;
    while (NextElement(element) <= end) {
        num++;
        element = NextElement(element);
    }
    return num;
}

// 0x0072ED54 (name after the overload)
const u8* nn::nwm::CTR::BssReader::GetVendorIe(const u8* oui) const
{
    const u8* found = NULL;
    const u8* element = GetIePointer();
    if (oui == NULL || element == NULL) {
        return found;
    }
    const u8* end = element + GetIeSize();
    if (NextElement(element) > end) {
        return found;
    }
    do {
        if (element[0] == ELEMENT_ID_VENDOR_SPECIFIC && memcmp(element + 2, oui, OUI_SIZE) == 0) {
            found = element;
        }
        element = NextElement(element);
    } while (NextElement(element) <= end);
    return found;
}

// 0x0072EE00 | fefates:bytes [tier B]
const u8* nn::nwm::CTR::BssReader::GetVendorIe(const u8* oui, u8 type) const
{
    const u8* found = NULL;
    const u8* element = GetIePointer();
    if (oui == NULL || element == NULL) {
        return found;
    }
    const u8* end = element + GetIeSize();
    if (NextElement(element) > end) {
        return found;
    }
    do {
        if (element[0] == ELEMENT_ID_VENDOR_SPECIFIC && memcmp(element + 2, oui, OUI_SIZE) == 0 && element[5] == type) {
            found = element;
        }
        element = NextElement(element);
    } while (NextElement(element) <= end);
    return found;
}

// 0x0072EEB8 (name is ours)
nn::nwm::Ssid nn::nwm::CTR::BssReader::GetSsid() const
{
    const u8* found = NULL;
    const u8* element = GetIePointer();
    if (element != NULL) {
        const u8* end = element + GetIeSize();
        if (NextElement(element) <= end) {
            do {
                if (element[0] == ELEMENT_ID_SSID) {
                    found = element;
                }
                element = NextElement(element);
            } while (NextElement(element) <= end);
            if (found != NULL && found[1] != 0) {
                return Ssid(found + 2, found[1]);
            }
        }
    }
    return Ssid();
}

} // namespace CTR
} // namespace nwm
} // namespace nn
