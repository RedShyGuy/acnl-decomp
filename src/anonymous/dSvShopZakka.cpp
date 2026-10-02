// Classes from the anonymous namespace of the original dSvShopZakka.cpp

#include "decomp.h"
#include "Other/dNoticeLoadAdapt.h"

namespace {

// vtable 0x008908E0 (vptr 0x008908E8), offset_to_top 0, 39 entries
class NoticeEyecatcher : public NoticeLoadAdapt
{
public:
    virtual void vf_0x00() {} // 0x0010BF3D slot 0x00 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x04() {} // 0x0010BF31 slot 0x04 | virtual slot, introduced by script::IMailRecept
};

// vtable 0x00890798 (vptr 0x008907A0), offset_to_top 0, 39 entries
class NoticeRenewal : public NoticeLoadAdapt
{
public:
    virtual void vf_0x00() {} // 0x003028E4 slot 0x00 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x04() {} // 0x00525208 slot 0x04 | virtual slot, introduced by script::IMailRecept
};

// vtable 0x0089083C (vptr 0x00890844), offset_to_top 0, 39 entries
class NoticeTimeService : public NoticeLoadAdapt
{
public:
    virtual void vf_0x00() {} // 0x00525228 slot 0x00 | virtual slot, introduced by script::IMailRecept
    virtual void vf_0x04() {} // 0x00525218 slot 0x04 | virtual slot, introduced by script::IMailRecept
};

} // namespace
