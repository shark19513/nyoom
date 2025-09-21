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

#include "Doppler.h"

#define MAX_OBJECT_POSITION 50.0f
#define MIN_PASS_BY_DISTANCE 1.0f
#define MAX_PASS_BY_DISTANCE 50.0f

/* prepare() should always be called before use */
Doppler::Doppler(float passByDistanceMeters, bool isLeftChannel) :
                m_delay(1.0f, 3000.0f),
                m_speedOfSound(343.0f),
                m_objectPositionMeters(0.0f),
                m_isLeftChannel(isLeftChannel),
                m_panning(0)
{
    /* method below initializes m_passByDistance, m_distanceMeters and m_maxDistanceMeters */
    setPassByDistance(passByDistanceMeters);
    /* set the direction of panning */
    if (m_isLeftChannel) {
        m_toLeft = 1.0f;
        m_toRight = 0.0f;
    } else {
        m_toLeft = 0.0f;
        m_toRight = 1.0f;
    }
}

Doppler::~Doppler() {}

void Doppler::prepare(float sampleRate) {
    m_delay.prepare(sampleRate);
}

void Doppler::applyEffect(const float& input, float& output){
    m_delay.processFrame(input, output);
}

void Doppler::setPassByDistance(float passByDistance) {
    /* passByDistance can range from 1 to 100 meters */
    if (passByDistance >= MIN_PASS_BY_DISTANCE && passByDistance <= MAX_PASS_BY_DISTANCE) {
        m_passByDistanceMeters = passByDistance;
        m_distanceMeters = calculateDistance(m_objectPositionMeters);
        m_maxDistanceMeters = calculateDistance(MAX_OBJECT_POSITION);
        calculateDelayTimeMillis();
    } else {
        std::cout << "-- Doppler::setPassByDistance --\n"
                  << "! invalid input !\n"
                  << "- please enter a value between 1.0 and 100.0 - \n";
    }
}

void Doppler::setObjectPosition(float sliderPosition) {
    /* accepts any signed value ranging from -1000 - 1000
     * the greater the value the greater the distance
     * stores only its absolute value */
    float position = std::abs(sliderPosition); //store the absolute value
    if (position <= MAX_OBJECT_POSITION) {
        m_objectPositionMeters = position;
        m_distanceMeters = calculateDistance(m_objectPositionMeters);
        calculateDelayTimeMillis();
        calculatePanning(sliderPosition); //this method does need the signed value though
        calculateAmplitude();
    } else {
        std::cout << "-- Doppler::setObjectPosition --\n"
                  << "! invalid input !\n"
                  << "- please enter a value between -" << MAX_OBJECT_POSITION
                  << " and " << MAX_OBJECT_POSITION << " -\n";
    }
}

float Doppler::calculateDistance(float objectPositionMeters) {
    /* Pythagoras go brrrrrrrr */
    const float a = m_passByDistanceMeters;
    const float b = objectPositionMeters;
    const float c = std::sqrt(a*a + b*b);

    return c;
}

void Doppler::calculateDelayTimeMillis() {
    /*  delay = distance / speed of sound * 1000  */
    const float delayTimeMillis = m_distanceMeters/m_speedOfSound * 1000.0f;
    m_delay.setDelayTimeMillis(delayTimeMillis);
}

void Doppler::calculatePanning(float sliderPosition) {
    /* scales the signed slider position value from -MAX_OBJECT_POSITION to MAX_OBJECT_POSITION
     * to create panning effect when paired with second instance of class */
    /* NOTE: panning is linear
     *  constant power panning would be more realistic */
    m_panning = Interpolation::mapInRange(sliderPosition,
                                        -MAX_OBJECT_POSITION, MAX_OBJECT_POSITION,
                                            m_toLeft, m_toRight);
}

void Doppler::calculateAmplitude() {
    /* fade-in/out logic
     * calculate amplitude based on m_objectPositionMeters
     * (which is the absolute value of the slider position)
     * multiply it with m_panning * 2
     * necessary because otherwise the amplitude only goes up to 0.5 */
    /* NOTE: amplitude fade-in/out is linear
     *  calculating based on distance and loudness of source would be more realistic */
    float amplitude = Interpolation::mapInRange(m_objectPositionMeters,
                                                0.0f, MAX_OBJECT_POSITION,
                                                1.0f, 0.0f);
    amplitude *= m_panning * 2;
    setAmplitude(amplitude);
}