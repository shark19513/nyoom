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

#include "Interpolation.h"

float Interpolation::nnMap(float value, float low, float high) {
  if(value < 0) {
    std::cout << "Value is lower than 0, please use values in range [0, 1]" << std::endl;
    return low;
  } else if(value > 1) {
    std::cout << "Value exceeds 1, please use values in range [0, 1]" << std::endl;
    return high;
  }
  //TODO: do something with about this warning
  if (static_cast<int>(value + 0.5f) == 0) {
    return low;
  }
  return high;
}

float Interpolation::linMap(float value, float low, float high) {
  if(value < 0) {
    std::cout << "Value is lower than 0, please use values in range [0, 1]" << std::endl;
    return low;
  } else if (value > 1) {
    std::cout << "Value exceeds 1, please use values in range [0, 1]" << std::endl;
    return high;
  }

  return low * (1 - value) + high * value;
}

float Interpolation::mapInRange(float value, float fromLow, float fromHigh, float toLow, float toHigh) {

  float partial = (value - fromLow) / (fromHigh - fromLow);
  float delta = toHigh - toLow;

  return toLow + delta * partial;
}