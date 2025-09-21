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

#include "Effect.h"

Effect::Effect() : m_bypassed(false), m_wetLevel(1), m_amplitude(1) {
}

Effect::~Effect() {
}

void Effect::processFrame(const float& input, float& output) {
    if (m_bypassed) {
        output = input;
    } else {
        // store dry and wet separately
        float drySignal = input;
        float wetSignal = 0.0f;

        applyEffect(input, wetSignal);
        // blend dry and wet and multiply with amplitude
        output = ((1.0f - m_wetLevel) * drySignal + m_wetLevel * wetSignal) * m_amplitude;
    }
}

void Effect::setWetLevel(float wetLevel) {
    if (wetLevel >= 0.0f && wetLevel <= 1.0f) {
        m_wetLevel = wetLevel;
    } else {
        std::cout << "- Effect::setWetLevel -\n"
        << "! invalid input !\n"
        << "- please enter a value between 0.0 and 1.0 -\n";
    }
}

void Effect::setBypassState(bool bypassed) {
    this->m_bypassed = bypassed;
}

void Effect::setAmplitude(float amplitude) {
    if (amplitude >= 0.0f && amplitude <= 1.0f) {
        m_amplitude = amplitude;
    } else {
        std::cout << "- Effect::setAmplitude -\n"
        << "! invalid input !\n"
        << "- please enter a value between 0.0 and 1.0 -\n";
    }
}

float Effect::getWetLevel() {
    return m_wetLevel;
}

bool Effect::getBypassState() {
    return m_bypassed;
}

float Effect::getAmplitude() {
    return m_amplitude;
}