#ifndef __ABSTRACT_SYSTEM_ALU__
#define __ABSTRACT_SYSTEM_ALU__
#include <cstdint>
#include <array>
#include <functional>

std::array<std::function<uint64_t(uint64_t,uint64_t)>,9> alu = {
    [](uint64_t x, uint64_t y) { return x+y; },
    [](uint64_t x, uint64_t y) { return x-y; },
    [](uint64_t x, uint64_t y) { return x*y; },
    [](uint64_t x, uint64_t y) { return x/y; },
    [](uint64_t x, uint64_t y) { return x && y ? 1 : 0; },
    [](uint64_t x, uint64_t y) { return x || y ? 1 : 0; },
    [](uint64_t x, uint64_t y) { return !x != !y; },
    [](uint64_t x, uint64_t y) { return x >> y; },
    [](uint64_t x, uint64_t y) { return x << y; }
};
#endif
