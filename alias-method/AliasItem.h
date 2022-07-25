/**
 * @author      : Serhii Bura (serhiibura@gmail.com)
 * @file        : AliasItem
 * @created     : Friday Jul 22, 2022 14:08:50 EEST
 */

#ifndef __ALIASITEM_H__
#define __ALIASITEM_H__

const float MAX_PROB = 1.0f;

template<typename ValueType>
class AliasItem {
private:
    const ValueType m_value;
    mutable float   m_prob;

public:
    explicit AliasItem(const ValueType value, float prob = MAX_PROB)
        : m_value{value}, m_prob{prob} {}

    bool updateProb(float prob) const
    {
        if (prob > MAX_PROB) {
            return false;
        }

        m_prob = prob;

        return true;
    }

    float getProb() const { return m_prob; }
    const ValueType getValue() const { return m_value; }

    ~AliasItem() = default;


    friend bool operator== (const AliasItem<ValueType>& item1, const AliasItem<ValueType>& item2)
    {
        return item1.m_value == item2.m_value;
    }

    friend bool operator< (const AliasItem<ValueType>& item1, const AliasItem<ValueType>& item2)
    {
        return item1.m_value < item2.m_value;
    }
};

#endif /* __ALIASITEM_H__ */
