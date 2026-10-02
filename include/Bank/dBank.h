#pragma once

#include "decomp.h"

// Instantiations found in the binary:
//   Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD178  vtable 0x008F834C
//   Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD180  vtable 0x008F8360
//   Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD188  vtable 0x008F8374
//   Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD190  vtable 0x008F8388
//   Bank<11776u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD198  vtable 0x008F839C
//   Bank<14156u, 2u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >  typeinfo 0x008CD1A0  vtable 0x008F83B0
//   Bank<15872u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1A8  vtable 0x008F83C4
//   Bank<17152u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1B0  vtable 0x008F83D8
//   Bank<18471u, 2u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >  typeinfo 0x008CD1B8  vtable 0x008F83EC
//   Bank<18597u, 2u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >  typeinfo 0x008CD1C0  vtable 0x008F8400
//   Bank<18647u, 2u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >  typeinfo 0x008CD1C8  vtable 0x008F8414
//   Bank<18944u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1D0  vtable 0x008F8428
//   Bank<25216u, 2u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >  typeinfo 0x008CD1D8  vtable 0x008F843C
//   Bank<26275u, 2u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >  typeinfo 0x008CD1E0  vtable 0x008F8450
//   Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD1E8  vtable 0x008F8464
//   Bank<32191u, 2u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >  typeinfo 0x008CD1F0  vtable 0x008F8478
//   Bank<33659u, 2u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >  typeinfo 0x008CD1F8  vtable 0x008F848C
//   Bank<35840u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD200  vtable 0x008F84A0
//   Bank<35840u, 1u, false, BankVramConfig<8704u, 6u, 1536u, 6u, false> >  typeinfo 0x008CD208  vtable 0x008F84B4
//   Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD210  vtable 0x008F84C8
//   Bank<66384u, 2u, false, BankVramConfig<66000u, 1u, 0u, 0u, false> >  typeinfo 0x008CD218  vtable 0x008F84DC
//   Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD220  vtable 0x008F84F0
//   Bank<8984u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >  typeinfo 0x008CD228  vtable 0x008F8504
//   Bank<8984u, 2u, false, BankVramConfig<8320u, 1u, 0u, 0u, false> >  typeinfo 0x008CD230  vtable 0x008F8518
//   Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >  typeinfo 0x008CD238  vtable 0x008F852C
template <auto T0, auto T1, auto T2, typename T3>
class Bank
{
public:
    // TODO: members unknown
};
