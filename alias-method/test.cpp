#include <cassert>
#include <iostream>
#include "AliasGenerator.h"

enum class Keys {
    eFirst = 0,
    eSecond,
    eThird,
    eFourth
};

int main()
{
    AliasGenerator<Keys> generator;
    generator.appendData(AliasItem<Keys>(Keys::eFirst));
    assert(false == generator.appendData(AliasItem<Keys>(Keys::eFirst)));
    assert(true == generator.updateProb(Keys::eFirst, 0.5));
    generator.appendData(AliasItem<Keys>(Keys::eSecond, 0.33));
    generator.appendData(AliasItem<Keys>(Keys::eThird, 0.083));
    generator.appendData(AliasItem<Keys>(Keys::eFourth, 0.083));

    auto value = generator.generateValue();

    switch (value) {
        case Keys::eFirst:
            std::cout << "First WIN!" << std::endl;

            break;
        case Keys::eSecond:
            std::cout << "Second WIN!" << std::endl;

            break;
        case Keys::eThird:
            std::cout << "Third WIN!" << std::endl;

            break;
        case Keys::eFourth:
            std::cout << "Fourth WIN!" << std::endl;

            break;
    }

    return 0;
}
