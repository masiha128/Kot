#pragma once


#include <kot/types.h>
#include <lib/node/node.h>
#include <heap/heap.h>


const char* to_string(uint64_t value);
const char* to_string(int64_t value);
const char* to_string(int value);
const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);
const char* to_string(double value, uint8_t decimalPlaces);
const char* to_string(double value);
const char* to_string(char c);

int strlen(char*p);
bool strcmp(char* a, char* b);
bool strcmp(char* a, char* b, size64_t size);

