#pragma once

#include <catch2/catch_all.hpp>
#include "WahFilter.h"

using namespace Catch;

/* FFT parameters */
constexpr int FFT_SIZE = 1024;
constexpr float SAMPLE_RATE = 48000.0f;
constexpr float FREQ_CUTOFF = 1000.0f;
constexpr float Q = 0.707f;

TEST_CASE("Test WahFilter", "[wahFilter]")
{
    WahFilter<float> lpf;
}
