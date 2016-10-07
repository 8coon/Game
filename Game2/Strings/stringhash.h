#ifndef STRINGHASH_H
#define STRINGHASH_H

#include "../Strings/string.h"


namespace std {
    template<> struct hash<coon::String>
    {
        std::size_t operator()(const coon::String& str) const {
            std::size_t seed = str.size();
            for (coon::Char ch: str) {
                seed ^= ch.a + 0x9e3779b9 + (seed >> 6) + (seed << 2);
            }
            return seed;
        }
    };
}

#endif // STRINGHASH_H
