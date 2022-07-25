/**
 * @author      : Serhii Bura (serhiibura@gmail.com)
 * @file        : AliasGenerator
 * @created     : Friday Jul 22, 2022 14:09:11 EEST
 */

#ifndef __ALIASGENERATOR_H__
#define __ALIASGENERATOR_H__

#include <iostream>
#include <algorithm>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "AliasItem.h"

template<typename ValueType>
class AliasGenerator {
public:
    using Data = AliasItem<ValueType>;
    using Values = std::set<Data>;

private:
    std::random_device m_rd;
    std::mt19937       m_gen{m_rd()};
    Values             m_values;
    std::vector<unsigned int> m_aliases;

    struct ProbItem {
        float prob;
        bool active;
    };

    std::vector<ProbItem> m_probabilities;

    bool m_generated;

    void generateTables();
public:
    AliasGenerator();
    AliasGenerator(Values & values);
    ~AliasGenerator();

    bool appendData(Data data);
    Data getData(ValueType value) const { return *m_values.find(value); }
    bool updateProb(ValueType value, float newProb);

    ValueType generateValue();
};

template<typename ValueType>
AliasGenerator<ValueType>::AliasGenerator()
{
    m_generated = false;
}

template<typename ValueType>
AliasGenerator<ValueType>::~AliasGenerator()
{
    m_values.clear();
}

template<typename ValueType>
AliasGenerator<ValueType>::AliasGenerator(Values & values)
    :m_values(values) {}

template<typename ValueType>
bool AliasGenerator<ValueType>::appendData(Data data)
{
    auto elem = m_values.find(data);
    if (elem != m_values.end()) {
        return false;
    }

    m_values.insert(data);

    m_generated = false;

    return true;
}

template<typename ValueType>
bool AliasGenerator<ValueType>::updateProb(ValueType value, float newProb)
{
    auto elem = m_values.find(AliasItem<ValueType>(value));
    if (elem == m_values.end()) {
        return false;
    }

    m_generated = false;

    return elem->updateProb(newProb);
}

template<typename ValueType>
void AliasGenerator<ValueType>::generateTables()
{
    auto data_begin = m_values.begin();
    auto data_end = m_values.end();
    size_t n = m_values.size();
    m_probabilities.clear();
    m_aliases.clear();

    m_probabilities.resize(n);
    m_aliases.resize(n);

    for(int i = 0; i < n; ++i) {
        auto data = data_begin;
        std::advance(data, i);
        m_probabilities[i].active = true;
        m_probabilities[i].prob = data->getProb() * n;
    }

    for(int j = 1; j < n; ++j) {
        auto l = std::find_if(m_probabilities.begin(), m_probabilities.end(),
                              [](const auto& item)
                              {
                                return item.prob <= 1 && true == item.active;
                              });
        auto g = std::find_if(m_probabilities.begin(), m_probabilities.end(),
                              [](const auto& item)
                              {
                                return item.prob >= 1 && true == item.active;
                              });
        while (g == l) {
            g = std::find_if(m_probabilities.begin(), m_probabilities.end(),
                              [](const auto& item)
                              {
                                return item.prob >= 1 && true == item.active;
                              });
        }

        unsigned int l_index = std::distance(m_probabilities.begin(), l);
        unsigned int g_index = std::distance(m_probabilities.begin(), g);

        m_aliases[l_index] = g_index;

        g->prob = g->prob - (1 - l->prob);
        l->active = false;
    }

    m_generated = true;
}

template<typename ValueType>
ValueType AliasGenerator<ValueType>::generateValue()
{
    auto data = m_values.begin();
    size_t n = m_values.size();

    if (false == m_generated) {
        generateTables();
    }

    std::random_device rseed;
    std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> roll_die(0, n - 1);
    int id = roll_die(rng);
    std::uniform_real_distribution<float> coin_flip(0, 1);
    float result = coin_flip(rng);

    auto item = m_probabilities.begin();
    std::advance(item, id);
    if (result < item->prob) {
        std::advance(data, std::distance(m_probabilities.begin(), item));
    } else {
        item = m_probabilities.begin();
        std::advance(item, m_aliases[id]);
        std::advance(data, std::distance(m_probabilities.begin(), item));
    }

    return data->getValue();
}

#endif /* __ALIASGENERATOR_H__ */
