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

#ifndef VALIDRAY_H
#define VALIDRAY_H

#include "Tympan/MetierSolver/AcousticRaytracer/Ray/Ray.h"
#include "Tympan/MetierSolver/AcousticRaytracer/Geometry/Shape.h"

/*!
* \namespace ValidRay
* \brief Cet espace de nom regroupe l'ensemble des fonctions n�cessaires � la validation d'un rayon.
*/
namespace ValidRay
{

bool validTriangleWithSpecularReflexion(Ray* r, Intersection* inter);
bool validCylindreWithDiffraction(Ray* r, Intersection* inter);
void appendDirectionToEvent(QSharedPointer<Event> e, TargetManager& targets);

};

#endif
