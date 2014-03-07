/*
 * Copyright (C) <2012> <EDF-R&D> <FRANCE>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "UnitConverter.h"
#include <iostream>

namespace Tools
{
void fromRadianToCarthesien(decimal theta, decimal phi, vec3& result)
{
    result.x = cos(theta) * cos(phi);
    result.y = cos(theta) * sin(phi);
    result.z = sin(theta);
}

void fromRadianToCarthesien2(decimal tetha, decimal phi, vec3& result)
{
    //std::cout<<"phi : "<<phi<<", tetha : "<<tetha<<std::endl;
    result.x = sin(tetha) * cos(phi);
    result.y = sin(tetha) * sin(phi);
    result.z = cos(tetha);
    return;
}
}
