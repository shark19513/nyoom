/**************************************************************************************************
This file is part of nyoom.

nyoom is free software: you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

nyoom is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with nyoom.
If not, see <https://www.gnu.org/licenses/>.
**************************************************************************************************/

#ifndef DELAY_H
#define DELAY_H
#include <iostream>
#include <cmath>
#include "Effect.h"
#include "Interpolation.h"

/* FRACTIONAL INTERPOLATING DELAY */
class Delay : public Effect {
public:
    Delay(float delayTimeMillis, float maxDelayMillis);
    virtual ~Delay() override;

    void prepare(float sampleRate) override;

    void applyEffect(const float& input, float& output) override;

    void setFeedback(float feedback);
    void setDelayTimeMillis(float delayTimeMillis);
    float getFeedback();
    float getDelayTimeMillis(); // returns TARGET delay time in ms

protected:
    float millisecondsToSamples(float millis);

    float samplesToMilliseconds(unsigned int samples);

    void setDelayTimeSamples(float delayTimeSamples);

    // increase write head and wrap if necessary
    inline void incrWriteH() {
        m_writeH++;
        wrapH(m_writeH);
    }
    inline void wrapH(unsigned int& head) {
        if (head >= m_bufferSize) head -= m_bufferSize;
    }

    void allocateBuffer();
    void releaseBuffer();

    float m_sampleRate;
    float m_delayTimeMillis; // stores delay time BEFORE interpolation
    float m_maxDelayTimeMillis;

    float m_delayTimeSamples; // current delay time (gets updated while smoothing)
    std::atomic<float> m_targetDelayTimeSamples; // to interpolate to
    std::atomic<float> m_smoothingStepSize; // calculated step size
    std::atomic<bool> m_theTimesTheyAreAChanging; // true if interpolation is currently happening

    float m_feedback;

    float* m_buffer;
    unsigned int m_bufferSize;
    unsigned int m_writeH;
};

#endif //DELAY_H
