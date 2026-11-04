#include "net/dTagWriter.h"

namespace net {
// 0x0050CFA8 (name is ours)
void net::TagWriter::AddIf(bool (*filter)(const wchar_t* text))
{
    if (m_pSourceTags != NULL) {
        const nn::nex::String* it = m_pSourceTags->begin();
        const nn::nex::String* end = m_pSourceTags->end();
        if (it != end) {
            do {
                if (filter(it->m_pString)) {
                    AddTag(m_pTags, it->m_pString);
                }
                it++;
            } while (it != end);
        }
    }
}

// 0x0050E57C (name is ours)
void AddTag(TagList* tags, const char* tag)
{
    tags->push_back(nn::nex::String(tag));
}

// 0x0050E5B4 (name is ours)
void AddTag(TagList* tags, const char* prefix, const wchar_t* text)
{
    nn::nex::String string(prefix);
    string += nn::nex::String(text);
    tags->push_back(string);
}

// 0x0050E608 (name is ours)
void AddTag(TagList* tags, const wchar_t* tag)
{
    tags->push_back(nn::nex::String(tag));
}
} // namespace net
