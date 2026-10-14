#include "nn/nex/_Type_variant/_Type_variant_Api.h"
#include "nn/nex/_Type_string/_Type_string_Api.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_Variant.h"

namespace nn {
namespace nex {
// 0x0036FC84 | fefates:bytes-fuzzy [tier B]
void nn::nex::_Type_variant::Add(nn::nex::Message* pMessage, const nn::nex::Variant& variant)
{
    u8 type = variant.m_Type;
    pMessage->AppendRaw(&type, sizeof(type));
    switch (variant.m_Type) {
    case Variant::TYPE_STRING:
        _Type_string::Add(pMessage, variant.GetStringValue());
        break;
    case Variant::TYPE_DOUBLE: {
        double value = variant.GetDoubleValue();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value));
        break;
    }
    case Variant::TYPE_INT64: {
        s64 value = variant.GetInt64Value();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value));
        break;
    }
    case Variant::TYPE_UINT64: {
        u64 value = variant.GetUInt64Value();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value));
        break;
    }
    case Variant::TYPE_BOOL: {
        bool value = variant.GetBoolValue();
        *pMessage << value;
        break;
    }
    case Variant::TYPE_DATE_TIME: {
        u64 value = variant.GetDateTimeValue().GetValue();
        pMessage->AppendRaw(reinterpret_cast<const u8*>(&value), sizeof(value));
        break;
    }
    default:
        break;
    }
}

// 0x0036FDD8 | fefates:bytes-fuzzy [tier B]
bool nn::nex::_Type_variant::Extract(nn::nex::Message* pMessage, nn::nex::Variant* pVariant)
{
    u8 type;
    pMessage->ExtractPrimitiveInline(&type);
    switch (type) {
    case Variant::TYPE_STRING: {
        String value;
        if (!_Type_string::Extract(pMessage, &value)) {
            return false;
        }
        *pVariant = Variant(value);
        break;
    }
    case Variant::TYPE_DOUBLE: {
        double value;
        pMessage->ExtractPrimitiveInline(&value);
        *pVariant = Variant(value);
        break;
    }
    case Variant::TYPE_INT64: {
        s64 value;
        pMessage->ExtractPrimitiveInline(&value);
        *pVariant = Variant(value);
        break;
    }
    case Variant::TYPE_UINT64: {
        u64 value;
        pMessage->ExtractPrimitiveInline(&value);
        *pVariant = Variant(value);
        break;
    }
    case Variant::TYPE_BOOL: {
        bool value;
        *pMessage >> value;
        *pVariant = Variant(value);
        break;
    }
    case Variant::TYPE_DATE_TIME: {
        DateTime value;
        u64 raw;
        pMessage->ExtractPrimitiveInline(&raw);
        value = DateTime(raw);
        *pVariant = Variant(value);
        break;
    }
    case Variant::TYPE_NONE:
        *pVariant = Variant();
        break;
    default:
        break;
    }
    return true;
}

} // namespace nex
} // namespace nn
