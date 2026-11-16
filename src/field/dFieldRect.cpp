#include "field/dFieldRect.h"

// 0x0052F720 (name is ours)
const Vector2i& Vector2i::Zero()
{
    // 0x00953BA4 (guard 0x00953BA0)
    static Vector2i s_Zero(0, 0);
    return s_Zero;
}

// 0x0052F7F0 | libgarden [tier A]
FieldRect::iterator& FieldRect::iterator::operator++()
{
    if (m_pRect != NULL && m_pEnd != NULL) {
        if (m_pEnd->x > m_Position.x) {
            m_Position.x++;
        } else {
            m_Position.x = m_pRect->min.x;
            m_Position.y++;
        }
    }
    return *this;
}

// 0x0052F858 (name is ours)
void FieldRect::Include(const Vector2i& pos)
{
    min.x = min.x < pos.x ? min.x : pos.x;
    min.y = min.y < pos.y ? min.y : pos.y;
    max.x = max.x > pos.x ? max.x : pos.x;
    max.y = max.y > pos.y ? max.y : pos.y;
}

// 0x00748EA4 | libgarden [tier A]
bool operator!=(const FieldRect::iterator& left, const FieldRect::iterator& right)
{
    bool isEqual = false;
    if (left.m_pRect != NULL && left.m_pEnd != NULL && right.m_pRect != NULL && right.m_pEnd != NULL &&
        right.m_Position.x == left.m_Position.x && right.m_Position.y == left.m_Position.y) {
        isEqual = true;
    }
    return !isEqual;
}

// 0x00748F04 | libgarden [tier A]
FieldRect::iterator FieldRect::end() const
{
    // one past the maximum
    iterator it(this, &max);
    it.m_Position.x = max.x;
    it.m_Position.y = max.y;
    if (it.m_pEnd != NULL) {
        if (it.m_pEnd->x > it.m_Position.x) {
            it.m_Position.x++;
        } else {
            it.m_Position.x = min.x;
            it.m_Position.y++;
        }
    }
    return it;
}

// 0x00748F3C | libgarden [tier A]
FieldRect::iterator FieldRect::begin() const
{
    iterator it(this, &max);
    it.m_Position.x = min.x;
    it.m_Position.y = min.y;
    return it;
}
