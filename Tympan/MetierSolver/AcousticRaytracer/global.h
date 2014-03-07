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

#ifndef GLOBAL_H
#define GLOBAL_H

////////////////////////////
// General Values
////////////////////////////
extern int globalMaxProfondeur;         //Nombre d'evenements autorises pour un rayon, globalMaxProfondeur inclu
extern int globalNbRaysPerSource;       //Nombre de rayons lances par les sources
extern float globalSizeReceiver;        //Diametre de la sphere representant le recepteur
extern int globalAccelerator;           //Choix de la structure acceleratrice. 0 : BruteForce, 1 : GridAccelerator, 2 : BVH, 3 : KdTree, other : GridAccelerator
extern int globalMaxTreeDepth;          //Profondeur maximale autorisee pour le BVH ou KdTree.
extern bool globalUseSol;               //Utilisation du sol (ou pas -cas NMPB-)
extern bool globalKeepDebugRay;         //Permet de conserver les rayons qui ont ete invalides pendant la propagation.
extern int globalDiscretization;        //Permet de choisir entre des rayons al�atoires ou d�terministes (discretisation source)

////////////////////////////
// NMPB value
///////////////////////////
extern int globalMaxReflexion;          //Nombre de reflexions speculaires autorisees pour un rayon, globalMaxReflexion inclu
extern int globalMaxDiffraction;        //Nombre de diffractions autorisees pour un rayon, globalMaxDiffraction inclu
extern float globalMaxLength;           //Longueur maximale autorisee pour un rayon, globalMaxLength inclu
extern float globalSampleGround2D;      //Echantillonage sur sol pour la description de la topograohie 2D sous le rayon.
extern float globalCylindreThick;       //Epaisseur des aretes de diffraction.

/////////////////////////////
// Targeting system + NMPB
/////////////////////////////
extern bool globalEnableTargets;        //Active la recherche de cible pour les sources. Pour le moment lie au solver NMPB.
extern bool globalEnableFullTargets;    //Active la recherche de cible apres un evenement diffu. Pour le moment lie au solver NMPB.
extern float globalTargetsDensity;      //Densite pour l'echantillonnage des zones interessantes.

//////////////////////////////
// Dtn extensions
/////////////////////////////
extern float globalAngleDiffMin;        //Angle minimal a prendre en compte entre 2 faces pour ajouter une arrete de diffraction
extern int globalNbRayWithDiffraction;  // Nombre de rayons relance lors d'un evenement diffraction
extern int globalRayTracingOrder;       //[0-2]Sens de traitement des rayon source-recepteur ou inverse (0 = SR / 1 =RS / 2 = auto)

extern bool globalUseMeteo;             // Prise en compte (ou non) de la geometrie
extern double globalAnalyticDMax;       // Distance de propagation maximale des rayons courbes
extern double globalAnalyticTMax;       // Temps de propagation maximal des rayons courbes
extern double globalAnalyticH;          // Pas de temps de calcul pour la propagation des rayons courbes
extern int globalAnalyticNbRay;         // Nombre de rayons tires pour le lancer de rayons courbes
extern float globalAnalyticAngleTheta;    // Angle de tir vertical (theta) des rayons

extern double globalAnalyticGradC;      // Gradient vertical de celerite
extern double globalAnalyticGradV;      // Gradient vertical de vitesse de vent
extern double globalAnalyticC0;         // Celerite du son initiale
extern int globalAnalyticTypeTransfo;   // Methode de transformation -- TOUJOURS = 1 -- pas d'autre methode definie
extern bool globalRestitModifiedGeom;   // Indique si l'on souhaite recuperer la geometrie transformee
extern double globalOverSampleD;            // [0 +[ (0 pas de surechantillonnage) Indique le taux de surechantillonnage des rayons
extern double globalWindDirection;          // Direction du vent (un vent a 0 est dirige du nord vers le sud)

extern bool globalUseFresnelArea;       // take into account the fresnel area
extern float globalAnime3DSigma;        // incertitude relative sur la taille du rayon au carree
extern float globalAnime3DForceC;       // Force C � 0.0 -> globalAnime3DForceC=0; 1.0 -> globalAnime3DForceC = 1 ou autre valeur d�pendant de globalAnime3DSigma
extern bool globalUsePostFilters;       // Utilisation (!=0) ou non (0) des filtres post lancer de rayons
#endif