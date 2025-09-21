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

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <iostream>


class Interpolation {
public:
  static float nnMap(float value, float low, float high);
  static float linMap(float value, float low, float high);
  static float mapInRange(float value, float fromLow,
            float fromHigh, float toLow, float toHigh);

protected:
  Interpolation() {}
  ~Interpolation() {}
};



#endif //INTERPOLATION_H
