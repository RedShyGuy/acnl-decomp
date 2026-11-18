#pragma once

#include "decomp.h"
#include "Bank/dBank.h"

// A bank of two slots that are used in turn (class name from the RTTI; the member name after libgarden, which
// places it in ExchangeBank; no code shows which of the two holds it): one slot is shown
// while the other one loads (bit 0 of m_ExchangeBit is the shown slot). No vtable of its own, all functions are inline.
// Instantiations found in the binary:
//   DoubleBank<14156u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >  typeinfo 0x008CB04C
//   DoubleBank<17152u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CB058
//   DoubleBank<18471u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >  typeinfo 0x008CB064
//   DoubleBank<18597u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >  typeinfo 0x008CB070
//   DoubleBank<18647u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >  typeinfo 0x008CB07C
//   DoubleBank<25216u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >  typeinfo 0x008CB088
//   DoubleBank<26275u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >  typeinfo 0x008CB094
//   DoubleBank<32191u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >  typeinfo 0x008CB0A0
//   DoubleBank<33659u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >  typeinfo 0x008CB0AC
//   DoubleBank<66384u, false, BankVramConfig<66000u, 1u, 0u, 0u, false> >  typeinfo 0x008CB0B8
//   DoubleBank<8984u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CB0C4
//   DoubleBank<8984u, false, BankVramConfig<8320u, 1u, 0u, 0u, false> >  typeinfo 0x008CB0D0
template <u32 HEAP_SIZE, bool FLAG, typename TVramConfig>
class DoubleBank : public Bank<HEAP_SIZE, 2, FLAG, TVramConfig>
{
public:
    DoubleBank() : m_ExchangeBit(0) {}

    // the slot that is shown
    u32 GetCurrentSlot() const { return m_ExchangeBit & 1; }

    u8 m_ExchangeBit; // 0x24
};
