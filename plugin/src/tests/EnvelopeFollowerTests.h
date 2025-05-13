#pragma once

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "EnvelopeFollower.h"

using namespace Catch;

TEST_CASE("Attack and decay coeffs are calculated properly", "[envelopeFollower]")
{
    double sampleRate = 44100.0;
    double tolerance = 0.0001f;

    /** Test for float */
    EnvelopeFollower<float> env_float;

    /** Make sure initial values are correct */
    REQUIRE_THAT(env_float.getAttackTime(), Catch::Matchers::WithinAbs(0.00001f, tolerance));
    REQUIRE_THAT(env_float.getDecayTime(), Catch::Matchers::WithinAbs(0.1f, tolerance));

    env_float.prepare(sampleRate);

    /** Make sure coeffs are properly calculated after prepare */
    REQUIRE_THAT(env_float.getAttackCoeff(), Catch::Matchers::WithinAbs(0.1035f, tolerance));
    REQUIRE_THAT(env_float.getDecayCoeff(), Catch::Matchers::WithinAbs(0.9997f, tolerance));

    /** Make sure coeffs are properly calculated after update */
    env_float.setAttackTime(0.1f);
    REQUIRE_THAT(env_float.getAttackCoeff(), Catch::Matchers::WithinAbs(0.9997f, tolerance));

    env_float.setDecayTime(0.00001f);
    REQUIRE_THAT(env_float.getDecayCoeff(), Catch::Matchers::WithinAbs(0.1035f, tolerance));

    /** Test for double */
    EnvelopeFollower<double> env_double;

    /** Make sure initial values are correct */
    REQUIRE_THAT(env_double.getAttackTime(), Catch::Matchers::WithinAbs(0.00001f, tolerance));
    REQUIRE_THAT(env_double.getDecayTime(), Catch::Matchers::WithinAbs(0.1f, tolerance));

    /** Make sure coeffs are properly calculated after prepare */
    env_double.prepare(sampleRate);
    REQUIRE_THAT(env_double.getAttackCoeff(), Catch::Matchers::WithinAbs(0.1035f, tolerance));
    REQUIRE_THAT(env_double.getDecayCoeff(), Catch::Matchers::WithinAbs(0.9997f, tolerance));

    /** Make sure coeffs are properly calculated after update */
    env_double.setAttackTime(0.1f);
    REQUIRE_THAT(env_double.getAttackCoeff(), Catch::Matchers::WithinAbs(0.9997f, tolerance));

    env_double.setDecayTime(0.00001f);
    REQUIRE_THAT(env_double.getDecayCoeff(), Catch::Matchers::WithinAbs(0.1035f, tolerance));
}
