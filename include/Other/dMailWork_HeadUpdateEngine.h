#pragma once

#include "decomp.h"
#include "Other/dMailWork.h"
#include "script/dMsgEngine.h"

// RTTI N8MailWork16HeadUpdateEngineE @ 0x008D401C
// vtable 0x0090C0BC (vptr 0x0090C0C4), offset_to_top 0, 8 entries
class MailWork::HeadUpdateEngine : public ::script::MsgEngine
{
public:
    HeadUpdateEngine(); // ctor address unknown
    virtual ~HeadUpdateEngine(); // 0x006AB160 slot 0x00
    // 0x006AB134 slot 0x04 (deleting dtor)
    virtual void Begin(); // 0x006AB06C slot 0x08
    virtual void End(); // 0x006AB02C slot 0x0C
    virtual void OnChar(const script::CharInfo* info); // 0x006AAF6C slot 0x14
    virtual void OnTag(const script::Tag* tag); // 0x006AB030 slot 0x18
};
