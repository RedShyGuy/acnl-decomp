#include "Exchange/dExchangeBank.h"
#include "Item/dResLoader.h"
#include "g3d/dResourceLoader.h"

// The instances of ExchangeBank (ARMCC keeps a copy of each function per instance).


// 0x007CB018 slot 0x00
// 0x007CB008 (deleting dtor)
template ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>::~ExchangeBank();
// 0x007CAF68 slot 0x08
template bool ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>::Release(unsigned int slot);
// 0x007CAF64 slot 0x0C
template void ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>::OnRequest(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CAF44 slot 0x10
template bool ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>::LoadResource(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CAF5C slot 0x14
template s32 ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>::LoadResourceSplit(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CAF4C slot 0x18
template bool ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>::Initialize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007CAF54 slot 0x1C
template bool ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>::Finalize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);

// 0x007CB4B8 slot 0x00
// 0x007CB4A8 (deleting dtor)
template ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::~ExchangeBank();
// 0x007CB430 slot 0x08
template bool ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Release(unsigned int slot);
// 0x007CB42C slot 0x0C
template void ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::OnRequest(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CB178 slot 0x10
template bool ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::LoadResource(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CB424 slot 0x14
template s32 ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CB180 slot 0x18
template bool ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007CB188 slot 0x1C
template bool ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);

// 0x007C8D54 slot 0x00
// 0x007C8D44 (deleting dtor)
template ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::~ExchangeBank();
// 0x007C8CA4 slot 0x08
template bool ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::Release(unsigned int slot);
// 0x007C8CA0 slot 0x0C
template void ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::OnRequest(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C8A14 slot 0x10
template bool ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::LoadResource(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C8C98 slot 0x14
template s32 ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::LoadResourceSplit(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C8A1C slot 0x18
template bool ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::Initialize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007C8A24 slot 0x1C
template bool ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::Finalize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);

// 0x007C8FE0 slot 0x00
// 0x007C8FD0 (deleting dtor)
template ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>::~ExchangeBank();
// 0x007C8ED8 slot 0x08
template bool ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>::Release(unsigned int slot);
// 0x007C8ED4 slot 0x0C
template void ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>::OnRequest(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C8EB4 slot 0x10
template bool ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>::LoadResource(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C8ECC slot 0x14
template s32 ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C8EBC slot 0x18
template bool ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot, bool isReady);
// 0x007C8EC4 slot 0x1C
template bool ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);

// 0x007C92C0 slot 0x00
// 0x007C92B0 (deleting dtor)
template ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::~ExchangeBank();
// 0x007C9210 slot 0x08
template bool ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::Release(unsigned int slot);
// 0x007C920C slot 0x0C
template void ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::OnRequest(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C91EC slot 0x10
template bool ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::LoadResource(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C9204 slot 0x14
template s32 ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::LoadResourceSplit(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C91F4 slot 0x18
template bool ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::Initialize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007C91FC slot 0x1C
template bool ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>::Finalize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);

// 0x007C94F4 slot 0x00
// 0x007C94E4 (deleting dtor)
template ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>::~ExchangeBank();
// 0x007C9444 slot 0x08
template bool ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>::Release(unsigned int slot);
// 0x007C9440 slot 0x0C
template void ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>::OnRequest(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C9420 slot 0x10
template bool ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>::LoadResource(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C9438 slot 0x14
template s32 ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>::LoadResourceSplit(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C9428 slot 0x18
template bool ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>::Initialize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot, bool isReady);
// 0x007C9430 slot 0x1C
template bool ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>::Finalize(const ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);

// 0x007C9780 slot 0x00
// 0x007C9770 (deleting dtor)
template ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::~ExchangeBank();
// 0x007C9678 slot 0x08
template bool ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::Release(unsigned int slot);
// 0x007C9674 slot 0x0C
template void ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::OnRequest(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C9654 slot 0x10
template bool ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::LoadResource(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C966C slot 0x14
template s32 ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007C965C slot 0x18
template bool ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot, bool isReady);
// 0x007C9664 slot 0x1C
template bool ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);

// 0x007C9AB8 slot 0x00
// 0x007C9AA8 (deleting dtor)
template ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::~ExchangeBank();
// 0x007C99B0 slot 0x08
template bool ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Release(unsigned int slot);
// 0x007C99AC slot 0x0C
template void ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::OnRequest(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C998C slot 0x10
template bool ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::LoadResource(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C99A4 slot 0x14
template s32 ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007C9994 slot 0x18
template bool ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007C999C slot 0x1C
template bool ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);

// 0x007CA04C slot 0x00
// 0x007CA03C (deleting dtor)
template ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::~ExchangeBank();
// 0x007C9EE0 slot 0x08
template bool ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::Release(unsigned int slot);
// 0x007C9EDC slot 0x0C
template void ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::OnRequest(const PlayerHeadBankKeyword& keyword, unsigned int slot);
// 0x007C9CC4 slot 0x10
template bool ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::LoadResource(const PlayerHeadBankKeyword& keyword, unsigned int slot);
// 0x007C9ED4 slot 0x14
template s32 ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::LoadResourceSplit(const PlayerHeadBankKeyword& keyword, unsigned int slot);
// 0x007C9CCC slot 0x18
template bool ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::Initialize(const PlayerHeadBankKeyword& keyword, unsigned int slot, bool isReady);
// 0x007C9CD4 slot 0x1C
template bool ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>::Finalize(const PlayerHeadBankKeyword& keyword, unsigned int slot);

// 0x007CA7AC slot 0x00
// 0x007CA79C (deleting dtor)
template ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::~ExchangeBank();
// 0x007CA698 slot 0x08
template bool ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::Release(unsigned int slot);
// 0x007CA694 slot 0x0C
template void ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::OnRequest(const PlayerCapBankKeyword& keyword, unsigned int slot);
// 0x007CA240 slot 0x10
template bool ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::LoadResource(const PlayerCapBankKeyword& keyword, unsigned int slot);
// 0x007CA68C slot 0x14
template s32 ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::LoadResourceSplit(const PlayerCapBankKeyword& keyword, unsigned int slot);
// 0x007CA248 slot 0x18
template bool ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::Initialize(const PlayerCapBankKeyword& keyword, unsigned int slot, bool isReady);
// 0x007CA250 slot 0x1C
template bool ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>::Finalize(const PlayerCapBankKeyword& keyword, unsigned int slot);

// 0x007CAAF4 slot 0x00
// 0x007CAAE4 (deleting dtor)
template ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::~ExchangeBank();
// 0x007CA9EC slot 0x08
template bool ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::Release(unsigned int slot);
// 0x007CA9E8 slot 0x0C
template void ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::OnRequest(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007CA9C8 slot 0x10
template bool ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::LoadResource(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007CA9E0 slot 0x14
template s32 ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);
// 0x007CA9D0 slot 0x18
template bool ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot, bool isReady);
// 0x007CA9D8 slot 0x1C
template bool ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>& keyword, unsigned int slot);

// 0x007CADAC slot 0x00
// 0x007CAD9C (deleting dtor)
template ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::~ExchangeBank();
// 0x007CAD24 slot 0x08
template bool ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Release(unsigned int slot);
// 0x007CAD20 slot 0x0C
template void ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::OnRequest(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CAD00 slot 0x10
template bool ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::LoadResource(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CAD18 slot 0x14
template s32 ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::LoadResourceSplit(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
// 0x007CAD08 slot 0x18
template bool ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Initialize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot, bool isReady);
// 0x007CAD10 slot 0x1C
template bool ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>::Finalize(const ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>& keyword, unsigned int slot);
