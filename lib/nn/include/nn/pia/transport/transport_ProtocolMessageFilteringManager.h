#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class ProtocolMessageFilteringManager
{
public:
    void CreateInstance(); // 0x0045F1C0 | fefates:bytes [tier B]
    void AddNoFilteringProtocolType(unsigned short); // 0x0045F294 | fefates:bytes [tier B]
    void RemoveNoFilteringProtocolType(unsigned short); // 0x0045F2BC | fefates:bytes [tier B]
    void IsFilteringEnabled(unsigned short) const; // 0x00736D10 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
