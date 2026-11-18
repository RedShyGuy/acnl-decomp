#include "Bank/dBank.h"

// The instances of Bank (ARMCC keeps a copy of each function per instance; the constructors are out of line only
// where an array of the bank is built with __aeabi_vec_ctor).

// 0x007F51E4
template Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Bank();
// 0x007F52A4 slot 0x00
// 0x007F521C (deleting dtor)
template Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F51DC slot 0x08
template bool Bank<1024u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F61C4 slot 0x00
// 0x007F613C (deleting dtor)
template Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F6134 slot 0x08
template bool Bank<3072u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F6B98 slot 0x00
// 0x007F6AEC (deleting dtor)
template Bank<8984u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F6AE4 slot 0x08
template bool Bank<8984u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F6EC8 slot 0x00
// 0x007F6E1C (deleting dtor)
template Bank<8984u, 2u, false, BankVramConfig<8320u, 1u, 0u, 0u, false> >::~Bank();
// 0x007F6C40 slot 0x08
template bool Bank<8984u, 2u, false, BankVramConfig<8320u, 1u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F5098
template Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Bank();
// 0x007F5158 slot 0x00
// 0x007F50D0 (deleting dtor)
template Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F5090 slot 0x08
template bool Bank<10240u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F5330
template Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Bank();
// 0x007F53F0 slot 0x00
// 0x007F5368 (deleting dtor)
template Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F5328 slot 0x08
template bool Bank<11059u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F5504 slot 0x00
// 0x007F547C (deleting dtor)
template Bank<11776u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F5474 slot 0x08
template bool Bank<11776u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F563C slot 0x00
// 0x007F5590 (deleting dtor)
template Bank<14156u, 2u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >::~Bank();
// 0x007F5588 slot 0x08
template bool Bank<14156u, 2u, true, BankVramConfig<300u, 2u, 2000u, 2u, false> >::Release(u32 slot);

// 0x007F5774 slot 0x00
// 0x007F56EC (deleting dtor)
template Bank<15872u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F56E4 slot 0x08
template bool Bank<15872u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F58AC slot 0x00
// 0x007F5800 (deleting dtor)
template Bank<17152u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F57F8 slot 0x08
template bool Bank<17152u, 2u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F5A08 slot 0x00
// 0x007F595C (deleting dtor)
template Bank<18471u, 2u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >::~Bank();
// 0x007F5954 slot 0x08
template bool Bank<18471u, 2u, true, BankVramConfig<600u, 2u, 3600u, 2u, false> >::Release(u32 slot);

// 0x007F5B64 slot 0x00
// 0x007F5AB8 (deleting dtor)
template Bank<18597u, 2u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >::~Bank();
// 0x007F5AB0 slot 0x08
template bool Bank<18597u, 2u, true, BankVramConfig<500u, 2u, 2800u, 2u, false> >::Release(u32 slot);

// 0x007F5CC0 slot 0x00
// 0x007F5C14 (deleting dtor)
template Bank<18647u, 2u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >::~Bank();
// 0x007F5C0C slot 0x08
template bool Bank<18647u, 2u, true, BankVramConfig<700u, 2u, 3600u, 2u, false> >::Release(u32 slot);

// 0x007F5DF8 slot 0x00
// 0x007F5D70 (deleting dtor)
template Bank<18944u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F5D68 slot 0x08
template bool Bank<18944u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F5F30 slot 0x00
// 0x007F5E84 (deleting dtor)
template Bank<25216u, 2u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >::~Bank();
// 0x007F5E7C slot 0x08
template bool Bank<25216u, 2u, false, BankVramConfig<736u, 4u, 3824u, 4u, false> >::Release(u32 slot);

// 0x007F608C slot 0x00
// 0x007F5FE0 (deleting dtor)
template Bank<26275u, 2u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >::~Bank();
// 0x007F5FD8 slot 0x08
template bool Bank<26275u, 2u, false, BankVramConfig<1800u, 4u, 6600u, 4u, false> >::Release(u32 slot);

// 0x007F62FC slot 0x00
// 0x007F6250 (deleting dtor)
template Bank<32191u, 2u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >::~Bank();
// 0x007F6248 slot 0x08
template bool Bank<32191u, 2u, true, BankVramConfig<2500u, 3u, 12400u, 3u, false> >::Release(u32 slot);

// 0x007F6458 slot 0x00
// 0x007F63AC (deleting dtor)
template Bank<33659u, 2u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >::~Bank();
// 0x007F63A4 slot 0x08
template bool Bank<33659u, 2u, true, BankVramConfig<2800u, 2u, 8800u, 2u, false> >::Release(u32 slot);

// 0x007F6590 slot 0x00
// 0x007F6508 (deleting dtor)
template Bank<35840u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F6500 slot 0x08
template bool Bank<35840u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F66A4 slot 0x00
// 0x007F661C (deleting dtor)
template Bank<35840u, 1u, false, BankVramConfig<8704u, 6u, 1536u, 6u, false> >::~Bank();
// 0x007F6614 slot 0x08
template bool Bank<35840u, 1u, false, BankVramConfig<8704u, 6u, 1536u, 6u, false> >::Release(u32 slot);

// 0x007F67B8 slot 0x00
// 0x007F6730 (deleting dtor)
template Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F6728 slot 0x08
template bool Bank<40543u, 1u, true, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F68F0 slot 0x00
// 0x007F6844 (deleting dtor)
template Bank<66384u, 2u, false, BankVramConfig<66000u, 1u, 0u, 0u, false> >::~Bank();
// 0x007F683C slot 0x08
template bool Bank<66384u, 2u, false, BankVramConfig<66000u, 1u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F69A0
template Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Bank();
// 0x007F6A60 slot 0x00
// 0x007F69D8 (deleting dtor)
template Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F6998 slot 0x08
template bool Bank<89600u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);

// 0x007F6F78
template Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >::Bank();
// 0x007F7038 slot 0x00
// 0x007F6FB0 (deleting dtor)
template Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >::~Bank();
// 0x007F6F70 slot 0x08
template bool Bank<96000u, 1u, false, BankVramConfig<20500u, 15u, 5500u, 15u, true> >::Release(u32 slot);

// 0x007F4F4C
template Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Bank();
// 0x007F500C slot 0x00
// 0x007F4F84 (deleting dtor)
template Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::~Bank();
// 0x007F4F44 slot 0x08
template bool Bank<100096u, 1u, false, BankVramConfig<0u, 0u, 0u, 0u, false> >::Release(u32 slot);
