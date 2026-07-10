/* AW-01 - AutoWah JUCE based audio plugin
   Copyright (C) 2026 Erwan Gateau-Magdeleine

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <cmath>

template <typename SampleType>
class EnvelopeFollower
{
public:
    //==============================================================================
    /** Constructor. */
    EnvelopeFollower()
    {
        updateAttackCoeff();
        updateDecayCoeff();
    }

    //==============================================================================
    /** Sets the attack time. */
    void setAttackTime(SampleType newAttackTime)
    {
        /* Convert milliseconds to seconds */
        attackTime = newAttackTime / static_cast<SampleType>(1000.0);
        updateAttackCoeff();
    }

    void setDecayTime(SampleType newDecayTime)
    {
        /* Convert milliseconds to seconds */
        decayTime = newDecayTime / static_cast<SampleType>(1000.0);
        updateDecayCoeff();
    }

    //==============================================================================
    /** Returns the attack time. */
    SampleType getAttackTime() const noexcept { return attackTime; }

    /** Returns the attack coefficient. */
    SampleType getAttackCoeff() const noexcept { return attackCoeff; }

    /** Returns the decay time. */
    SampleType getDecayTime() const noexcept { return decayTime; }

    /** Returns the decay coefficient. */
    SampleType getDecayCoeff() const noexcept { return decayCoeff; }

    //==============================================================================
    /** Initializes the envelope follower. */
    void prepare (double newSampleRate)
    {
        sampleRate = newSampleRate;

        updateAttackCoeff();
        updateDecayCoeff();

        reset();
    }

    /** Reset the internal state of the enveloppe follower. */
    void reset()
    {
        delayTap = 0.0f;
    }

    /** Process a single sample of data */
    SampleType process (SampleType sample)
    {
        SampleType coeff = sample > delayTap ? attackCoeff : decayCoeff;

        SampleType yn = coeff * delayTap + (static_cast<SampleType>(1.0) - coeff) * std::abs(sample);
        delayTap = yn;

        return yn;
    }

private:
    //==============================================================================
    void updateAttackCoeff()
    {
        attackCoeff = static_cast<SampleType> (std::exp (-1.0 / (attackTime * sampleRate)));
    }

    void updateDecayCoeff()
    {
        decayCoeff = static_cast<SampleType> (std::exp (-1.0 / (decayTime * sampleRate)));
    }

    double sampleRate;

    SampleType attackTime =  static_cast<SampleType>(0.00001f);
    SampleType decayTime = static_cast<SampleType>(0.1f);

    SampleType attackCoeff;
    SampleType decayCoeff;

    SampleType delayTap;
};
