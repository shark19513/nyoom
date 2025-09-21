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

#ifndef DOPPLER_H
#define DOPPLER_H

#include "Effect.h"
#include "Delay.h"
#include "Interpolation.h"
#include <cmath>
#include <iostream>

/* DELAY BASED DOPPLER EFFECT
 * meant to be combined with second instance for stereo panning
 * takes bool that flips panning direction */
class Doppler : public Effect {
public:
    Doppler(float passByDistanceMeters, bool isLeftChannel);
    ~Doppler() override;

    void prepare(float sampleRate) override;
    void applyEffect(const float& input, float& output) override;

    void setPassByDistance(float passByDistance);
    void setObjectPosition(float sliderPosition);

private:
    float calculateDistance(float objectPositionMeters);
    void calculateDelayTimeMillis();
    void calculatePanning(float sliderPosition);
    void calculateAmplitude();

    Delay m_delay;

    float m_speedOfSound;

    /* distance from sound source object in meters */
    float m_distanceMeters;
    float m_maxDistanceMeters; //max value distance can be with current pass by distance

    /* distance from trajectory in meters */
    std::atomic<float> m_passByDistanceMeters;

    /* position of the object in the trajectory in meters
     * expressed as the absolute distance from the center */
    std::atomic<float> m_objectPositionMeters;

    /* fields used for panning logic*/
    const bool m_isLeftChannel;
    float m_toLeft;
    float m_toRight;
    float m_panning;
};


#endif //DOPPLER_H
