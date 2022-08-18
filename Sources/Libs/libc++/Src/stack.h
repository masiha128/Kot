#include <kot/heap.h>

#ifndef __LIBCPP__STACK__
#define __LIBCPP__STACK__ 1

namespace std {

    class Stack {
    private:
        uint64_t top;
        uint64_t table_size;
        uintptr_t current_table = NULL;
    public:
        Stack(uint64_t table_size);
        uint8_t pop8();
        uint16_t pop16();
        uint32_t pop32();
        uint64_t pop64();
        void push8(uint8_t item);
        void push16(uint16_t item);
        void push32(uint32_t item);
        void push64(uint64_t item);
    };

};

#endif