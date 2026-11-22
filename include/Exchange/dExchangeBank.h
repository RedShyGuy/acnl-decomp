#pragma once

#include "decomp.h"
#include "Double/dDoubleBank.h"
#include "Exchange/dExchangeBankBase.h"
#include "Exchange/dExchangeBankKeyword.h"

// A double bank whose slots are exchanged by keyword (class name from the RTTI; the slot names and m_Current/
// m_ExchangeBit after libgarden, the other names are ours): the derived banks load the resources of a keyword into a
// slot (LoadResource/LoadResourceSplit), set them up (Initialize) and release them (Finalize). m_LoadedBits marks the slots that hold loaded resources.
// Instantiations found in the binary:
//   ExchangeBank<14156u, BankVramConfig<300u, 2u, 2000u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>  typeinfo 0x008CB614  vtable 0x008EE4A4
//   ExchangeBank<17152u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, false>  typeinfo 0x008CB634  vtable 0x008EE4CC
//   ExchangeBank<18471u, BankVramConfig<600u, 2u, 3600u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, true>  typeinfo 0x008CB654  vtable 0x008EE4F4
//   ExchangeBank<18597u, BankVramConfig<500u, 2u, 2800u, 2u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32765>, true>  typeinfo 0x008CB674  vtable 0x008EE51C
//   ExchangeBank<18647u, BankVramConfig<700u, 2u, 3600u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>  typeinfo 0x008CB694  vtable 0x008EE544
//   ExchangeBank<25216u, BankVramConfig<736u, 4u, 3824u, 4u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>  typeinfo 0x008CB6B4  vtable 0x008EE56C
//   ExchangeBank<26275u, BankVramConfig<1800u, 4u, 6600u, 4u, false>, PlayerHeadBankKeyword, false>  typeinfo 0x008CB6D4  vtable 0x008EE594
//   ExchangeBank<32191u, BankVramConfig<2500u, 3u, 12400u, 3u, false>, PlayerCapBankKeyword, true>  typeinfo 0x008CB6F4  vtable 0x008EE5BC
//   ExchangeBank<33659u, BankVramConfig<2800u, 2u, 8800u, 2u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32765>, true>  typeinfo 0x008CB714  vtable 0x008EE5E4
//   ExchangeBank<66384u, BankVramConfig<66000u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>  typeinfo 0x008CB734  vtable 0x008EE60C
//   ExchangeBank<8984u, BankVramConfig<0u, 0u, 0u, 0u, false>, ExchangeBankKeywordFgNameEx<(SvFgName::Name)32766>, false>  typeinfo 0x008CB754  vtable 0x008EE634
//   ExchangeBank<8984u, BankVramConfig<8320u, 1u, 0u, 0u, false>, ExchangeBankKeywordMyDesignFgNameEx<(SvFgName::Name)32766>, false>  typeinfo 0x008CB774  vtable 0x008EE65C
template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
class ExchangeBank : public ExchangeBankBase, public DoubleBank<HEAP_SIZE, FLAG, TVramConfig>
{
public:
    typedef DoubleBank<HEAP_SIZE, FLAG, TVramConfig> Base;

    ExchangeBank()
    {
        m_LoadedBits[0] = 0;
        Reset();
    }
    DECOMP_NOINLINE inline virtual ~ExchangeBank();

    // the resources of the slot are unloaded (Finalize) if it holds some, the keyword of the slot is cleared
    virtual bool Release(u32 slot) override; // slot 0x08
    // a request for keyword into the slot starts (empty in all banks)
    virtual void OnRequest(const TKeyword& keyword, u32 slot); // slot 0x0C
    // the loading of the resources of keyword into the slot starts (false: not possible)
    virtual bool LoadResource(const TKeyword& keyword, u32 slot); // slot 0x10
    // the state of the loading: 0 still loading, 1 loaded, 2 failed or nothing to load
    virtual s32 LoadResourceSplit(const TKeyword& keyword, u32 slot); // slot 0x14
    // the models of the loaded resources (false without isReady or on failure)
    virtual bool Initialize(const TKeyword& keyword, u32 slot, bool isReady); // slot 0x18
    // the resources of the slot are released (false: not yet)
    virtual bool Finalize(const TKeyword& keyword, u32 slot); // slot 0x1C

    bool IsLoaded(u32 slot) const { return slot < 2 && ((m_LoadedBits[slot >> 3] >> (slot & 7)) & 1) != 0; }
    void ClearLoaded(u32 slot) { m_LoadedBits[slot >> 3] &= ~(1 << (slot & 7)); }

    // (inline) the resources of the slot are released; the heap is cleared when they are (it is marked as disposing
    // meanwhile)
    bool ReleaseSlot(u32 slot)
    {
        sead::Heap* heap = this->GetAllocator(slot)->m_pHeap;
        if (heap == NULL) {
            return false;
        }
        heap->mFlag |= sead::Heap::FLAG_DISPOSING;
        bool isReleased;
        if (Release(slot)) {
            heap->freeAll();
            isReleased = true;
        } else {
            isReleased = false;
        }
        heap->mFlag &= ~sead::Heap::FLAG_DISPOSING;
        return isReleased;
    }
    // (inline) the slots are released, then the heaps destroyed
    DECOMP_ALWAYS_INLINE void FinalizeBank()
    {
        for (u32 i = 0; i < 2; i++) {
            ReleaseSlot(i);
        }
        ClearRequest();
        Base::FinalizeBank();
    }
    // the results of Exchange (names are ours)
    enum ExchangeResult
    {
        EXCHANGE_RESULT_LOADING = 0, // still loading
        EXCHANGE_RESULT_SAME = 1,    // nothing new (keyword is shown already, or the setup failed)
        EXCHANGE_RESULT_SHOWN = 2,   // keyword is shown now
        EXCHANGE_RESULT_LOADED = 3,  // (only without isApply) keyword is loaded and waits for a call with isApply
    };

    // (inline) keyword is shown (into the slot that is not shown, then the slots change): ExchangeResult; without isApply
    // a loaded keyword is not set up yet (m_IsSetupPending), so that several banks can change in the same frame
    DECOMP_ALWAYS_INLINE s32 Exchange(const TKeyword& keyword, bool isApply = true)
    {
        return ExchangeImpl(keyword, isApply);
    }
    DECOMP_ALWAYS_INLINE s32 ExchangeImpl(const TKeyword& keyword, bool isApply)
    {
        const u32 next = ~this->m_ExchangeBit & 1;
        if (!m_IsLoading && m_Keywords[this->GetCurrentSlot()].IsSame(keyword)) {
            return 1;
        }
        // (the original also requests the same keyword again while it loads, unless its setup is pending)
        if ((!m_IsLoading && !m_IsSetupPending) || !m_Current.IsSame(keyword) || !m_IsSetupPending) {
            if (next >= 2 || (this->m_Allocators[next].m_pHeap != NULL && Release(next))) {
                m_Current = keyword;
                m_IsLoading = true;
                m_Keywords[next] = TKeyword();
                m_IsSetupPending = false;
                OnRequest(m_Current, next);
            }
        }
        bool isLoaded = false;
        if (m_IsLoading) {
            m_LoadResult = LoadResourceSplit(m_Current, next);
            SetLoaded(next);
            if (m_LoadResult == 0) {
                return 0;
            }
            if (!isApply) {
                m_IsSetupPending = true;
            }
            isLoaded = true;
            m_IsLoading = false;
        }
        if (m_IsSetupPending) {
            if (!isApply) {
                return EXCHANGE_RESULT_LOADED;
            }
            m_IsSetupPending = false;
        } else if (!isLoaded) {
            return 0;
        }
        const bool isSetUp = Initialize(m_Current, next, m_LoadResult == 1);
        m_Keywords[next] = m_Current;
        this->m_ExchangeBit++;
        m_Current = TKeyword();
        m_IsLoading = false;
        return isSetUp ? 2 : 1;
    }
    void SetLoaded(u32 slot)
    {
        if (slot < 2) {
            m_LoadedBits[slot >> 3] |= 1 << (slot & 7);
        }
    }

    // (inline) no request
    void ClearRequest()
    {
        m_Current = TKeyword();
        m_IsLoading = false;
        m_IsSetupPending = false;
        m_LoadResult = 1;
    }
    // (inline) no requests, empty keywords, nothing loaded
    void Reset()
    {
        ClearRequest();
        for (u32 i = 0; i < 2; i++) {
            m_Keywords[i] = TKeyword();
        }
        m_LoadedBits[0] = 0;
    }

    TKeyword m_Current; // 0x26 (0x28 for the My Design keyword)
    bool m_IsLoading;      // the request is loading
    s8 m_IsSetupPending;   // loaded, the setup waits for Exchange with isApply
    u8 m_LoadResult;       // of LoadResourceSplit
    TKeyword m_Keywords[2];
    u8 m_LoadedBits[1];
};

template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
inline ExchangeBank<HEAP_SIZE, TVramConfig, TKeyword, FLAG>::~ExchangeBank()
{
    Reset();
}

template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
inline bool ExchangeBank<HEAP_SIZE, TVramConfig, TKeyword, FLAG>::Release(u32 slot)
{
    TKeyword& keyword = m_Keywords[slot];
    if (IsLoaded(slot)) {
        if (!Finalize(keyword, slot)) {
            return false;
        }
        ClearLoaded(slot);
    }
    keyword = TKeyword();
    return true;
}

template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
inline void ExchangeBank<HEAP_SIZE, TVramConfig, TKeyword, FLAG>::OnRequest(const TKeyword& /* keyword */, u32 /* slot */)
{
    // empty (in the original too)
}

template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
inline bool ExchangeBank<HEAP_SIZE, TVramConfig, TKeyword, FLAG>::LoadResource(const TKeyword& keyword, u32 slot)
{
    return true;
}

template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
inline s32 ExchangeBank<HEAP_SIZE, TVramConfig, TKeyword, FLAG>::LoadResourceSplit(const TKeyword& keyword, u32 slot)
{
    return 1;
}

template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
inline bool ExchangeBank<HEAP_SIZE, TVramConfig, TKeyword, FLAG>::Initialize(const TKeyword& keyword, u32 slot, bool isReady)
{
    return true;
}

template <u32 HEAP_SIZE, typename TVramConfig, typename TKeyword, bool FLAG>
inline bool ExchangeBank<HEAP_SIZE, TVramConfig, TKeyword, FLAG>::Finalize(const TKeyword& /* keyword */, u32 /* slot */)
{
    return true;
}
