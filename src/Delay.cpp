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

#include "Delay.h"

#define MAX_STEP_SIZE 3 // maximum step size
#define NUM_SMOOTHING_STEPS 11025 // delta gets divided by this number inside setDelayTimeSamples

Delay::Delay(float delayTimeMillis, float maxDelayTimeMillis) :
    m_sampleRate(0), m_delayTimeMillis(delayTimeMillis),
    m_maxDelayTimeMillis(maxDelayTimeMillis),
    m_delayTimeSamples(0), m_targetDelayTimeSamples(0),
    m_smoothingStepSize(0), m_theTimesTheyAreAChanging(false),
    m_feedback(0), m_buffer(nullptr),
    m_bufferSize(0), m_writeH(0)
{
    // prepare() should always be called before use
}

Delay::~Delay() {
    releaseBuffer();
}

void Delay::prepare(float sampleRate) {
    this->m_sampleRate = sampleRate;
    // init the buffer size to the max delay time and init the delay time
    m_bufferSize = static_cast<unsigned int>(millisecondsToSamples(m_maxDelayTimeMillis));
    allocateBuffer();
    setDelayTimeMillis(m_delayTimeMillis);
}

void Delay::applyEffect(const float& input, float& output)
{
    if (m_theTimesTheyAreAChanging) {
        m_delayTimeSamples += m_smoothingStepSize;
        // calculate current difference
        float delta = m_targetDelayTimeSamples - m_delayTimeSamples;
        // check if we reached the target or are close enough to make a jump to it
        /* NOTE: i tried using m_smoothingStepSize instead of 1.0f here but that messed it up
         *  there might be a better way to do this */
        if (delta > -1.0f && delta < 1.0f) {
            m_delayTimeSamples = m_targetDelayTimeSamples;
            m_theTimesTheyAreAChanging = false;
        }
    }

    // calculate read head based on position of write head
    float readPos = static_cast<float>(m_writeH) - m_delayTimeSamples + static_cast<float>(m_bufferSize);
    unsigned int readH = static_cast<unsigned int>(readPos); //get rid of fractional part for read head
    unsigned int nextReadHPos = readH + 1; // indicates next element in buffer
    float readHFraction = readPos - static_cast<float>(readH); // store fractional part separately
    wrapH(readH);
    wrapH(nextReadHPos);

    /* interpolate between the current buffer element and the next one with readHFraction as value */
    output = Interpolation::linMap(readHFraction, m_buffer[readH], m_buffer[nextReadHPos]);

    // write input to write head position together with the feedback from the output
    m_buffer[m_writeH] = input + output * m_feedback;
    incrWriteH(); // tick write head
}

void Delay::setFeedback(float feedback) {
    if (feedback >= 0.0f && feedback <= 1.0f) {
        this->m_feedback = feedback;
    } else {
        std::cout << "-- Delay::setFeedback --\n"
                  << "! invalid input !\n"
                  << "- please enter a value between 0.0 and 1.0 -\n";
    }
}

void Delay::setDelayTimeMillis(float delayTimeMillis) {
    /* delay time must be 0.1ms minimum to prevent interpolation from 0 to 0 */
    if (delayTimeMillis > 0.1f && delayTimeMillis < m_maxDelayTimeMillis ) {
        m_delayTimeMillis = delayTimeMillis;
        setDelayTimeSamples(millisecondsToSamples(m_delayTimeMillis));
    } else {
        std::cout << "-- Delay::setDelayTime -- \n"
                  << "! invalid input !\n"
                  << "- please enter a value between " << "0.1"
                  << " and " << m_maxDelayTimeMillis-1 << " -\n";
    }
}

float Delay::getFeedback() {
    return m_feedback;
}

float Delay::getDelayTimeMillis() {
    return m_delayTimeMillis;
}

float Delay::millisecondsToSamples(float millis) {
    float delayTimeSamples = millis / 1000.0f * m_sampleRate;
    return delayTimeSamples;
}

float Delay::samplesToMilliseconds(unsigned int samples) {
    return static_cast<float>(samples) / m_sampleRate * 1000.0f;
}

void Delay::allocateBuffer() {
    // allocate buffer and set all samples to 0
    m_buffer = new float[m_bufferSize];
    for (unsigned int i = 0; i < m_bufferSize; i++) {
        m_buffer[i] = 0;
    }
}

void Delay::releaseBuffer() {
    delete[] m_buffer;
    m_buffer = nullptr;
}

// set the number of samples to delay with interpolation smoothing
void Delay::setDelayTimeSamples(float delayTimeSamples) {
    constexpr float epsilon = 0.0001f; // error margin
    // do if the new delay time is different from the current (epsilon is error margin)
    if (std::abs(delayTimeSamples - m_delayTimeSamples) > epsilon) {
        // calculate smooth interpolation
        m_theTimesTheyAreAChanging = true; // gets set to false in applyEffect
        m_targetDelayTimeSamples = delayTimeSamples; // set target
        // calculate delta and divide by a number of steps
        m_smoothingStepSize = (m_targetDelayTimeSamples - m_delayTimeSamples)
                                    / NUM_SMOOTHING_STEPS;

        // check if step size exceeds MAX_STEP_SIZE
        if (m_smoothingStepSize > 0 && m_smoothingStepSize > MAX_STEP_SIZE) {
        // if the new time is greater than it was before and exceeds the max step size
            m_smoothingStepSize = MAX_STEP_SIZE;
        } else if (m_smoothingStepSize < 0 && m_smoothingStepSize < -MAX_STEP_SIZE) {
        // if the new time is smaller than it was before and exceeds the negative max step size
            m_smoothingStepSize = -MAX_STEP_SIZE;
        }
    }
}