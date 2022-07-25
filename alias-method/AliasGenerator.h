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

public:
    AliasGenerator() = default;
    AliasGenerator(Values & values);
    ~AliasGenerator();

    bool appendData(Data data);
    Data getData(ValueType value) const { return *m_values.find(value); }
    bool updateProb(ValueType value, float newProb);

    ValueType generateValue();
};

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

    return true;
}

template<typename ValueType>
bool AliasGenerator<ValueType>::updateProb(ValueType value, float newProb)
{
    auto elem = m_values.find(AliasItem<ValueType>(value));
    if (elem == m_values.end()) {
        return false;
    }

    return elem->updateProb(newProb);
}

template<typename ValueType>
ValueType AliasGenerator<ValueType>::generateValue()
{
    auto data_begin = m_values.begin();
    auto data_end = m_values.end();
    size_t n = m_values.size();
    struct ProbItem {
        float prob;
        bool active;
    };
    std::vector<ProbItem> probabilities(n);
    std::vector<float> alias(n);

    for(int i = 0; i < n; ++i) {
        auto data = data_begin;
        std::advance(data, i);
        probabilities[i].active = true;
        probabilities[i].prob = data->getProb() * n;
    }

    for(int j = 1; j < n; ++j) {
        auto l = std::find_if(probabilities.begin(), probabilities.end(),
                              [](const auto& item)
                              {
                                return item.prob <= 1 && true == item.active;
                              });
        auto g = std::find_if(probabilities.begin(), probabilities.end(),
                              [](const auto& item)
                              {
                                return item.prob >= 1 && true == item.active;
                              });
        while (g == l) {
            g = std::find_if(probabilities.begin(), probabilities.end(),
                              [](const auto& item)
                              {
                                return item.prob >= 1 && true == item.active;
                              });

        }

        unsigned int l_index = std::distance(probabilities.begin(), l);
        unsigned int g_index = std::distance(probabilities.begin(), g);
        alias[l_index] = g_index;

        g->prob = g->prob - (1 - l->prob);
        l->active = false;
    }

    std::random_device rseed;
    std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> roll_die(0, n - 1);
    int id = roll_die(rng);
    std::uniform_real_distribution<float> coin_flip(0, 1);
    float result = coin_flip(rng);

    std::cout << "Column: " << id << " Result: " << result << std::endl;

    auto data = data_begin;
    auto item = probabilities.begin();
    std::advance(item, id);
    if (result < item->prob) {
        std::advance(data, std::distance(probabilities.begin(), item));
    } else {
        item = probabilities.begin();
        std::advance(item, alias[id]);
        std::advance(data, std::distance(probabilities.begin(), item));
    }

    return data->getValue();
}

#endif /* __ALIASGENERATOR_H__ */
