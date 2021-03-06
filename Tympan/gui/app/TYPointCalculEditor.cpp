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
 * \file TYPointCalculEditor.cpp
 * \brief gestion de l'edition d'un point de calcul
 */


#include <qmessagebox.h>
#include <qinputdialog.h>

#include "Tympan/models/business/TYProjet.h"
#include "Tympan/models/business/TYPointControl.h"
#include "Tympan/models/business/OLocalizator.h"
#include "Tympan/gui/app/TYApplication.h"
#include "Tympan/gui/app/TYActions.h"
#include "Tympan/gui/app/TYModelerFrame.h"
#include "Tympan/gui/app/TYSiteModelerFrame.h"
#include "Tympan/gui/app/TYRenderWindowInteractor.h"
#include "TYPointCalculEditor.h"


#define TR(id) OLocalizator::getString("TYPointCalculEditor", (id))


TYPointCalculEditor::TYPointCalculEditor(TYSiteModelerFrame* pModeler) :
    TYAbstractSceneEditor(pModeler)
{
    _active = false;

    slotViewTypeChanged(_pModeler->getCurrentView());
}

TYPointCalculEditor::~TYPointCalculEditor()
{
}

void TYPointCalculEditor::undo()
{
}

void TYPointCalculEditor::slotMouseReleased(int x, int y, Qt::MouseButton button, Qt::KeyboardModifiers state)
{
    if ((button == Qt::LeftButton) && _active)
    {
        LPTYProjet pProjet = NULL;

        if (getTYApp()->getCurProjet())
        {
            pProjet = getTYApp()->getCurProjet();
        }

        if (pProjet)
        {
            if (_pModeler->askForResetResultat())
            {
                // Hauteur pour ce point de controle
                bool ok = false;
                double hauteur = QInputDialog::getDouble(_pModeler, "", TR("id_msg_gethauteur"), 2.0, -1000.0, 1000.0, 2, &ok);

                if (ok)
                {
                    double point[4];
                    LPTYPointControl pPointControl = new TYPointControl();

                    // Pos
                    displayToWorld(x, _pInteractor->height() - y, 0.0, point);

                    // Si la grille magnetique est activee
                    if (_pModeler->getSnapGridActive())
                    {
                        snapToGrid(point[0], point[1], point[2]);
                    }

                    // Init point
                    pPointControl->setHauteur(hauteur);
                    pPointControl->setFromOGL(point);
                    pPointControl->_z = 0.0;

                    // Action
                    TYAction* pAction = new TYAddPointControlAction(pProjet, pPointControl, _pModeler, TR("id_action_addptcalcul"));
                    _pModeler->getActionManager()->addAction(pAction);

                    // Ajout
                    pProjet->addPointControl(pPointControl);
                    pProjet->updateGraphicTree();
                    refreshProjectFrame();
                    _pModeler->getView()->getRenderer()->updateDisplayList();
                    _pModeler->updateView();
                    TYElement::setIsSavedOk(true);
                }
            }
        }
        else
        {
            // Msg "Pas de projet..."
            QString msg(TR("id_warning_no_curcalcul"));
            writeOutputMsg(msg);
            QMessageBox::warning(_pModeler, "Tympan", msg, QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
}

void TYPointCalculEditor::slotViewTypeChanged(int view)
{
    if (view == TYModelerFrame::TopView)
    {
        _active = true;
    }
    else
    {
        _active = false;
    }
}
