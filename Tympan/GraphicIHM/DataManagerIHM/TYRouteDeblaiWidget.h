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
 *
 */

#ifndef __TY_ROUTE_DEBLAI_WIDGET__
#define __TY_ROUTE_DEBLAI_WIDGET__


#include "TYWidget.h"
//Added by qt3to4:
#include <QGridLayout>
#include <QLabel>

class TYRouteDeblai;
class QLineEdit;
class QGridLayout;
class QLabel;
class QGroupBox;
class QPushButton;
class TYAcousticLineWidget;
class QCheckBox;
class QComboBox;

/**
 * Widget pour la classe TYRouteDeblai.
 */
class TYRouteDeblaiWidget : public TYWidget
{
    Q_OBJECT

    TY_DECL_METIER_WIDGET(TYRouteDeblai)

    // Methodes
public:
    /**
     * Constructeur.
     */
    TYRouteDeblaiWidget(TYRouteDeblai* pElement, QWidget* _pParent = NULL);
    /**
     * Destructeur.
     */
    virtual ~TYRouteDeblaiWidget();


public slots:
    virtual void updateContent();
    virtual void apply();


    // Membres
protected:
    QGroupBox* _groupBox;
    QLabel* _labelVitMoy;
    QLineEdit* _lineEditVitMoy;
    QLabel* _labelTrafic;
    QComboBox* _comboBoxTrafic;
    QGroupBox* _groupBoxTraficJour;
    QLineEdit* _pLineEditDebitPLJour;
    QLineEdit* _pLineEditDebitVLJour;
    QGroupBox* _groupBoxTraficNuit;
    QLineEdit* _pLineEditDebitPLNuit;
    QLineEdit* _pLineEditDebitVLNuit;

    QGridLayout* _routeLayout;
    QGridLayout* _groupBoxLayout;
    QGridLayout* _groupBoxTraficJourLayout;
    QGridLayout* _groupBoxTraficNuitLayout;

    TYAcousticLineWidget* _elmW;
};


#endif // __TY_ROUTE_DEBLAI_WIDGET__
