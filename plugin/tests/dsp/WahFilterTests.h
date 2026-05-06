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
    impulse[0] = 1.0f;

    /* Create response */
    std::vector<float> response(IR_LENGTH);

    /* Create magnitude vector*/
    std::vector<float> magnitude;

    SECTION("Low-pass filter functions properly")
    {
        WahFilter<float> lpf;
        lpf.setMorphing(0.0f);
        lpf.setCenterFrequency(FREQ_CUTOFF);
        lpf.setResonance(Q);
        lpf.prepare(SAMPLE_RATE);

        /* Process the signal using temp as the process is in place */
        for (int i = 0; i < IR_LENGTH; i++)
        {
            response[i] = lpf.process(impulse[i]);
        }

        getFFTResponseFromSignal(response, magnitude);

        float magDC = magnitude[0];
        float magCutOff = getMagnitudeAtFrequency(magnitude, FREQ_CUTOFF, SAMPLE_RATE, IR_LENGTH);
        float magNyquist = magnitude[IR_LENGTH / 2];

        REQUIRE(magDC > 0.9f);
        REQUIRE(magNyquist < 0.1f);
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
            response[i] = bpf.process(impulse[i]);
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
        hpf.setMorphing(1.0);
        hpf.setCenterFrequency(FREQ_CUTOFF);
        hpf.setResonance(Q);
        hpf.prepare(SAMPLE_RATE);

        /* Process the signal using temp as the process is in place */
        for (int i = 0; i < IR_LENGTH; i++)
        {
            response[i] = hpf.process(impulse[i]);
        }

        getFFTResponseFromSignal(response, magnitude);

        float magDC = magnitude[0];
        float magCutOff = getMagnitudeAtFrequency(magnitude, FREQ_CUTOFF, SAMPLE_RATE, IR_LENGTH);
        float magNyquist = magnitude[IR_LENGTH / 2];

        REQUIRE(magDC < 0.1);
        REQUIRE(magNyquist > 0.9);
        REQUIRE(magCutOff > magDC);
    }
}

TEST_CASE("Band Pass Magnitude", "[wahfilter]")
{
    WahFilter<float> filter;
    filter.setMorphing(0.5f);
    filter.setCenterFrequency(FREQ_CUTOFF);
    filter.setResonance(Q);
    filter.prepare(SAMPLE_RATE);

    SECTION("DC blocked")
    {
        auto mag = filter.getMagnitudeFromFrequency(0.0f);
        REQUIRE(mag == Approx(0.0f).margin(0.001f));
    }

    SECTION("Center Freq at 0dB")
    {
        auto mag = filter.getMagnitudeFromFrequency(FREQ_CUTOFF);
        REQUIRE(mag == Approx(1.0f).margin(0.001f));
    }

}

TEST_CASE("Low Pass Magnitude", "[wahfilter]")
{
    WahFilter<float> filter;
    filter.setMorphing(0.0f);
    filter.setCenterFrequency(FREQ_CUTOFF);
    filter.setResonance(Q);
    filter.prepare(SAMPLE_RATE);

    SECTION("DC is fully passed")
    {
        auto mag = filter.getMagnitudeFromFrequency(0.0f);
        REQUIRE(mag == Approx(1.0f).margin(0.001f));
    }

    SECTION("Cutoff is at -3dB")
    {
        auto mag = filter.getMagnitudeFromFrequency(FREQ_CUTOFF);
        REQUIRE(mag == Approx(0.707f).margin(0.001f));
    }

    SECTION("One octave above cutoff is attenuated ~-12dB")
    {
        auto mag = filter.getMagnitudeFromFrequency(2.0f * FREQ_CUTOFF);
        REQUIRE(mag == Approx(0.25f).margin(0.01f));
    }

    SECTION("Nyquist is fully blocked")
    {
        auto mag = filter.getMagnitudeFromFrequency(SAMPLE_RATE / 2.0f);
        REQUIRE(mag == Approx(0.0f).margin(0.001f));
    }
}

TEST_CASE("High Pass Magnitude", "[wahfilter]")
{
    WahFilter<float> filter;
    filter.setMorphing(1.0f);
    filter.setCenterFrequency(FREQ_CUTOFF);
    filter.setResonance(Q);
    filter.prepare(SAMPLE_RATE);

    SECTION("DC is fully blocked")
    {
        auto mag = filter.getMagnitudeFromFrequency(0.0f);
        REQUIRE(mag == Approx(0.0f).margin(0.001f));
    }

    SECTION("Cutoff is at -3dB")
    {
        auto mag = filter.getMagnitudeFromFrequency(FREQ_CUTOFF);
        REQUIRE(mag == Approx(0.707f).margin(0.001f));
    }

    SECTION("One octave above cutoff is attenuated ~-12dB")
    {
        auto mag = filter.getMagnitudeFromFrequency(FREQ_CUTOFF / 2.0f);
        REQUIRE(mag == Approx(0.25f).margin(0.01f));
    }

    SECTION("Nyquist is fully passed")
    {
        auto mag = filter.getMagnitudeFromFrequency(SAMPLE_RATE / 2);
        REQUIRE(mag == Approx(1.0f).margin(0.001f));
    }
}