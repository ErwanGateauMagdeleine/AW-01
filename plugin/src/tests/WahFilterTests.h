#pragma once

#include <catch2/catch_all.hpp>
#include "WahFilter.h"
#include "kissFFTUtility.h"

using namespace Catch;

/* FFT parameters */
constexpr int IR_LENGTH = 1024;
constexpr double SAMPLE_RATE = 48000.0;
constexpr float FREQ_CUTOFF = 1000.0f;
constexpr float Q = 0.707f;

TEST_CASE("Test WahFilter", "[wahFilter]")
{
    /* Generate an impulse */
    std::vector<float> impulse(IR_LENGTH, 0);
    impulse[0] = 1;

    /* Create response */
    std::vector<float> response(IR_LENGTH);

    /* Create magnitude vector*/
    std::vector<float> magnitude;

    SECTION("Low-pass filter functions properly")
    {
        WahFilter<float> lpf;
        lpf.setMorphing(0.0);
        lpf.setCenterFrequency(FREQ_CUTOFF);
        lpf.setResonance(Q);
        lpf.prepare(SAMPLE_RATE);

        /* Process the signal using temp as the process is in place */
        for (int i = 0; i < IR_LENGTH; i++)
        {
            float sample = impulse[i];
            lpf.process(&sample);
            response[i] = sample;
        }

        getFFTResponseFromSignal(response, magnitude);

        float magDC = magnitude[0];
        float magCutOff = getMagnitudeAtFrequency(magnitude, FREQ_CUTOFF, SAMPLE_RATE, IR_LENGTH);
        float magNyquist = magnitude[IR_LENGTH / 2];

        REQUIRE(magDC > 0.5);
        REQUIRE(magNyquist < 0.1);
        REQUIRE(magCutOff > magNyquist);
    }

    SECTION("Band-Pass filter functions properly")
    {
        WahFilter<float> bpf;
        bpf.setMorphing(0.5);
        bpf.setCenterFrequency(FREQ_CUTOFF);
        bpf.setResonance(Q);
        bpf.prepare(SAMPLE_RATE);

        /* Process the signal using temp as the process is in place */
        for (int i = 0; i < IR_LENGTH; i++)
        {
            float sample = impulse[i];
            bpf.process(&sample);
            response[i] = sample;
        }

        getFFTResponseFromSignal(response, magnitude);

        float magDC = magnitude[0];
        float magCutOff = getMagnitudeAtFrequency(magnitude, FREQ_CUTOFF, SAMPLE_RATE, IR_LENGTH);
        float magNyquist = magnitude[IR_LENGTH / 2];

        REQUIRE(magCutOff > magDC);
        REQUIRE(magCutOff > magNyquist);
        REQUIRE(magCutOff > 0.3);
    }

    SECTION("High-Pass filter functions properly")
    {
        WahFilter<float> hpf;
        hpf.setMorphing(0.5);
        hpf.setCenterFrequency(FREQ_CUTOFF);
        hpf.setResonance(Q);
        hpf.prepare(SAMPLE_RATE);

        /* Process the signal using temp as the process is in place */
        for (int i = 0; i < IR_LENGTH; i++)
        {
            float sample = impulse[i];
            hpf.process(&sample);
            response[i] = sample;
        }

        getFFTResponseFromSignal(response, magnitude);

        float magDC = magnitude[0];
        float magCutOff = getMagnitudeAtFrequency(magnitude, FREQ_CUTOFF, SAMPLE_RATE, IR_LENGTH);
        float magNyquist = magnitude[IR_LENGTH / 2];

        REQUIRE(magDC < 0.1);
        REQUIRE(magNyquist > 0.5);
        REQUIRE(magCutOff > magDC);
    }
}
