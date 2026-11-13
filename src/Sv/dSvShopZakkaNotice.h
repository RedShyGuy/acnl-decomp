#pragma once

// The notices of the shop of the Nooklings on the bulletin board: classes of the anonymous namespace of the original
// dSvShopZakka.cpp (class names from the RTTI, all other names are ours). The original file has an ARM part
// (dSvShopZakka.cpp) and a Thumb part (dSvShopZakkaThumb.cpp); both include this header.

#include "decomp.h"
#include "Other/dNoticeLoadAdapt.h"

namespace {

// the notice of a renewal (the date of the renewal is the month and day word)
// RTTI N40_GLOBAL__N__16_dSvShopZakka_cpp_9075ecaf13NoticeRenewalE
// vtable 0x00890798 (vptr 0x008907A0)
class NoticeRenewal : public NoticeLoadAdapt
{
public:
    ~NoticeRenewal() override;

    // the message is written with this receiver (Begin, the words, Load, End); true: written
    bool Write(SvBulletinBoardMessage* message, const char* file, s32 label, const nn::fnd::DateTimeParameters* date,
               const script::IWord* name, const script::IWord* town); // 0x0010BEB4 (Thumb)
};

// the notice of the time service, the half price sale of the day (the hour is the word of the tag 4/16)
// RTTI N40_GLOBAL__N__16_dSvShopZakka_cpp_9075ecaf17NoticeTimeServiceE
// vtable 0x0089083C (vptr 0x00890844)
class NoticeTimeService : public NoticeLoadAdapt
{
public:
    ~NoticeTimeService() override;

    bool Write(SvBulletinBoardMessage* message, const char* file, s32 label, const nn::fnd::DateTimeParameters* date,
               const script::IWord* name, const script::IWord* town); // 0x0010BF44 (Thumb)
};

// RTTI N40_GLOBAL__N__16_dSvShopZakka_cpp_9075ecaf16NoticeEyecatcherE
// vtable 0x008908E0 (vptr 0x008908E8)
class NoticeEyecatcher : public NoticeLoadAdapt
{
public:
    ~NoticeEyecatcher() override;
};

} // namespace
