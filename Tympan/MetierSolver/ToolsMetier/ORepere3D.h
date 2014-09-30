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

/*
 *
 *
 *
 */

#ifndef __O_REPERE__
#define __O_REPERE__


#include "OPoint3D.h"
#include "OVector3D.h"
#include "OMatrix.h"


/**
 * \file ORepere3D.h
 * \class ORepere3D
 * \brief 3D Repere with a point and 3 vectors.
 * \author Projet_Tympan
 *
 */
class ORepere3D
{
    // Methods
public:
    /**
     * Default constructor.
     */
    ORepere3D();

    /**
     * Copy constructor.
     *
     * \param repere The object to be copied.
     */
    ORepere3D(const ORepere3D& repere);

    /**
     * \brief Constructor with a point and 3 vectors.
     *
     * \param origin The origin point.
     * \param vecI Vector I for the X axis.
     * \param vecJ Vector J for the Y axis.
     * \param vecK Vector K for the Z axis.
     */
    ORepere3D(const OPoint3D& origin, const OVector3D& vecI, const OVector3D& vecJ, const OVector3D& vecK);

    /**
     * \brief Constructor with a point and 1 vectors.
     *  build an arbitrary 3D repere from a point and a single vector
     */
    ORepere3D(const OPoint3D& origin, const OVector3D& vec);


    /**
     * Constructor from a matrix
     *
     * \param matrix
     */
    ORepere3D(const OMatrix& matrix);

    /**
     * Destructor.
     */
    virtual ~ORepere3D();

    /**
     * Assignment operator.
     *
     * \param repere The source to be assigned.
     */
    ORepere3D& operator=(const ORepere3D& repere);

    /**
     * The equality operator.
     *
     * \param repere The object reference with which this object is compared.
     */
    bool operator==(const ORepere3D& repere) const;

    /**
     * The inequality operator.
     *
     * \param repere The object reference with which this object is compared.
     */
    bool operator!=(const ORepere3D& repere) const;

    /**
     * \fn void set(const OPoint3D& origin, const OVector3D& vecI, const OVector3D& vecJ, const OVector3D& vecK);
     * \brief Sets with a point and 3 vectors.
     *
     * \param origin The origin point.
     * \param vecI Vector I for the X axis.
     * \param vecJ Vector J for the Y axis.
     * \param vecK Vector K for the Z axis.
     */
    void set(const OPoint3D& origin, const OVector3D& vecI, const OVector3D& vecJ, const OVector3D& vecK);

    /**
     * \fn void set(const OMatrix& matrix);
     * \brief Sets from a matrix.
     *
     * \param matrix A matrix to set this repere with.
     */
    void set(const OMatrix& matrix);

    /**
     * \fn void normalize();
     * \brief Normalize each vectors composing this repere.
     */
    void normalize();

    /**
     * \return The homogeneous matrix associated with this pose (aka ORepere)
     *
     * \brief return the transformation matrix from unity to this pose
     * such as this = transform * unity
     */
    OMatrix asMatrix() const;

    //Members
public:
    ///The origine point.
    OPoint3D _origin;
    ///Vector I for the X axis.
    OVector3D _vecI;
    ///Vector J for the Y axis.
    OVector3D _vecJ;
    ///Vector K for the Z axis.
    OVector3D _vecK;

};

#endif //__O_REPERE__
