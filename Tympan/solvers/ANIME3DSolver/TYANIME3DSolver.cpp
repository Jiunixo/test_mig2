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

#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>

#include "Tympan/models/common/3d.h"
#include "Tympan/models/common/delaunay_maker.h"
#include "Tympan/models/business/TYAcousticModelInterface.h"
#include "Tympan/models/business/TYTrajet.h"
#include "Tympan/models/business/TYCalcul.h"
#include "Tympan/models/business/infrastructure/TYSiteNode.h"
#include "Tympan/models/solver/acoustic_problem_model.hpp"
#include "Tympan/models/solver/acoustic_result_model.hpp"
#include "Tympan/solvers/AcousticRaytracer/global.h"
#include "TYANIME3DRayTracerSetup.h"
#include "TYANIME3DAcousticModel.h"
#include "TYANIME3DAcousticPathFinder.h"
#include "TYANIME3DFaceSelector.h"
#include "TYANIME3DSolver.h"


typedef std::vector<TYTrajet> TabTrajet;

TYANIME3DSolver::TYANIME3DSolver()
{
    _tabPolygon = NULL;
}

TYANIME3DSolver::~TYANIME3DSolver()
{
    purge();
}

void TYANIME3DSolver::purge()
{
    if (_tabPolygon)
    {
        delete [] _tabPolygon;
    }

    _tabPolygon = NULL;
}

void TYANIME3DSolver::init(const TYSiteNode& site, TYCalcul& calcul)
{
    site.getInfrastructure()->getAllSrcs(&calcul, _tabSources);

    calcul.getAllRecepteurs(_tabRecepteurs); // recuperation des recepteurs

    _tabRay.clear();
}

bool TYANIME3DSolver::solve(const TYSiteNode& site, TYCalcul& calcul,
        const tympan::AcousticProblemModel& aproblem,
        tympan::AcousticResultModel& aresult)
{
    // Rcupration (once for all) des sources et des rcepteurs
    init(site, calcul);

    // Construction de la liste des faces utilise pour le calcul
    TYANIME3DFaceSelector fs(site);
    bool bRet = fs.exec(_tabPolygon, _tabPolygonSize);

    if (!bRet) { return false; }

    // Ray tracing computation
    TYANIME3DAcousticPathFinder apf(_tabPolygon, _tabPolygonSize, _tabSources, _tabRecepteurs, _tabRay);
    apf.exec();

    ////////////////////////////////////////////////////////////
    // Calculs acoustiques sur les rayons via la methode ANIME3D
    ////////////////////////////////////////////////////////////

    TYANIME3DAcousticModel aam(calcul, site, _tabRay, _tabPolygon, _tabSources, _tabRecepteurs);

    // calcul de la matrice de pression totale pour chaque couple (S,R)
    OTab2DSpectreComplex tabSpectre = aam.ComputeAcousticModel();

    TabTrajet& trajets = calcul.getTabTrajet();  // recuperation du tableau de trajets

    trajets.clear(); // nettoyage des trajets

    OSpectre sLP; // puissance de la source et spectre de pression pour chaque couple (S,R)
    TYTrajet traj;

    for (int i = 0; i < _tabSources.size(); i++) // boucle sur les sources
    {
        for (int j = 0; j < _tabRecepteurs.size(); j++) // boucle sur les recepteurs
        {
            tabSpectre[i][j].setEtat(SPECTRE_ETAT_LIN);
            sLP = tabSpectre[i][j];
            sLP.setType(SPECTRE_TYPE_LP);

            tabSpectre[i][j] = sLP.toDB();  // conversion du tableau resultat en dB

            traj.setSourcePonctuelle(_tabSources[i]);
            traj.setPointCalcul(_tabRecepteurs[j]);
            traj.setSpectre(tabSpectre[i][j]);
            trajets.push_back(traj);
        }
    }

    if (globalUseMeteo && globalOverSampleD)
    {
        for (unsigned int i = 0; i < _tabRay.size(); i++)
        {
            _tabRay[i]->tyRayCorrection( apf.get_geometry_modifier() );
        }
    }

//    calcul.setTabRays(_tabRay);

    // BEGIN : COMPLEMENTS "DECORATIFS"
    ostringstream fic_out;
    fic_out << "rayons_infos.txt" << ends;
    ofstream fic(fic_out.str().c_str());
    fic << "on a " << _tabRay.size() << " rayons dans cette scene" << endl; // nombre de rayons
    fic.close();
    // END : COMPLEMENTS "DECORATIFS"

    return true;
}