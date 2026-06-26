#pragma once

#include <cstdint>
#include <cstddef>

class Speaker
{
public:

    bool begin();

    bool play(const int32_t *buffer, size_t samples);

};