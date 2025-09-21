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

#ifndef EFFECT_H
#define EFFECT_H
#include <iostream>


class Effect {
public:
    Effect();
    virtual ~Effect();

    virtual void prepare(float sampleRate) = 0;
    void processFrame(const float& input, float& output);

    void setWetLevel(float wetLevel);
    void setBypassState(bool state);
    void setAmplitude(float amplitude);
    float getWetLevel();
    bool getBypassState();
    float getAmplitude();

protected:
    virtual void applyEffect(const float& input, float& output) = 0;

    bool m_bypassed;
    float m_wetLevel;
    float m_amplitude;
};



#endif //EFFECT_H
