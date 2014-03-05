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

/**
 * \file TYRectangularMaillageWidget.h
 * \brief outil IHM pour un maillage rectangulaire (fichier header)
 * \author Projet_Tympan
 *
 *
 *
 *
 *
 */

#ifndef __TY_RECTANGULAR_MAILLAGE_WIDGET__
#define __TY_RECTANGULAR_MAILLAGE_WIDGET__


#include "TYWidget.h"

class TYRectangularMaillage;
class QLineEdit;
class TYMaillageWidget;

/**
 * \class TYRectangularMaillageWidget
 * \brief classe de l'objet IHM pour un maillage rectangulaire
 */
class TYRectangularMaillageWidget : public TYWidget
{
    Q_OBJECT

    TY_DECL_METIER_WIDGET(TYRectangularMaillage)

    // Methodes
public:
    /**
     * Constructeur.
     */
    TYRectangularMaillageWidget(TYRectangularMaillage* pElement, QWidget* _pParent = NULL);
    /**
     * Destructeur.
     */
    virtual ~TYRectangularMaillageWidget();


public slots:
    virtual void updateContent();
    virtual void apply();

    /**
     * Mets a jour le nombre de points que contiendra le maillage
     * en fonction des parametres entres.
     */
    void updateNbPoints();

    /**
     * Edite la widget du rectangle.
     */
    void editRectangle();


    // Membres
protected:
    QLineEdit* _lineEditNomRectangle;
    QLineEdit* _lineEditSurface;
    QLineEdit* _lineEditDensiteX;
    QLineEdit* _lineEditDensiteY;

    TYMaillageWidget* _maillageW;
};


#endif // __TY_RECTANGULAR_MAILLAGE_WIDGET__
