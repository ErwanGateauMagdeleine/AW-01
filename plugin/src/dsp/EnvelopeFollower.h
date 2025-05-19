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
        attackTime = newAttackTime;
        updateAttackCoeff();
    }

    void setDecayTime(SampleType newDecayTime)
    {
        decayTime = newDecayTime;
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
    void process (SampleType* sample)
    {
        auto coeff = (*sample) > delayTap ? attackCoeff : decayCoeff;

        (*sample) = coeff * delayTap + (1.0 - coeff) * (*sample);
        delayTap = (*sample);
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
