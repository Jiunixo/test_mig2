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

#ifndef __TY_ROUTE__
#define __TY_ROUTE__


#include "Tympan/MetierSolver/DataManagerMetier/ComposantGeoAcoustique/TYAcousticLine.h"
#include "Tympan/MetierSolver/DataManagerMetier/ComposantAcoustique/TYTrafic.h"

#if TY_USE_IHM
#include "Tympan/GraphicIHM/DataManagerIHM/TYRouteWidget.h"
#include "Tympan/GraphicIHM/DataManagerGraphic/TYRouteGraphic.h"
#endif

#include "RoadEmissionNMPB08.h"


///Noeud geometrique de type TYRoute.
typedef TYGeometryNode TYRouteGeoNode;
///Smart Pointer sur TYRouteGeoNode.
typedef SmartPtr<TYRouteGeoNode> LPTYRouteGeoNode;
///Collection de noeuds geometriques de type TYRoute.
typedef std::vector<LPTYRouteGeoNode> TYTabRouteGeoNode;

/**
 * Comprend les proprietes acoustiques et geometriques d'une route.
 *
 * @author Projet_Tympan
 *
 */
class TYRoute: public TYAcousticLine
{
    OPROTOSUPERDECL(TYRoute, TYAcousticLine)
    TY_EXTENSION_DECL(TYRoute)
    TY_EXT_GRAPHIC_DECL(TYRoute)

    // Methodes
public:

    enum TrafficRegimes { Day, Evening, Night, NB_TRAFFIC_REGIMES} ;

    /**
     * Constructeur.
     */
    TYRoute();
    /**
     * Constructeur par copie.
     */
    TYRoute(const TYRoute& other);
    /**
     * Destructeur.
     */
    virtual ~TYRoute();

    ///Operateur =.
    TYRoute& operator=(const TYRoute& other);
    ///Operateur ==.
    bool operator==(const TYRoute& other) const;
    ///Operateur !=.
    bool operator!=(const TYRoute& other) const;

    virtual bool deepCopy(const TYElement* pOther, bool copyId = true);

    virtual std::string toString() const;

    virtual DOM_Element toXML(DOM_Element& domElement);
    virtual int fromXML(DOM_Element domElement);

    /**
     * Set/Get de la vitesse moyenne.
     */
    double getVitMoy() const { return _vitMoy; }

    /**
     * Set/Get de la vitesse moyenne.
     */
    void setVitMoy(double vit) { _vitMoy = vit; }

    /**
     * Set/Get du trafic de jour.
     */
    const TYTrafic& getTraficJour() const { return traffic_regimes[Day]; }
    TYTrafic& getTraficJour() { return traffic_regimes[Day]; }

    /**
     * Set/Get du trafic de jour.
     */
    //  void setTraficJour(const LPTYTrafic pTrafic);

    /**
     * Set/Get du trafic de nuit.
     */
    const TYTrafic& getTraficNuit() const { return traffic_regimes[Night]; }
    TYTrafic& getTraficNuit() { return traffic_regimes[Night]; }

    /**
     * Set/Get du trafic de nuit.
     */
    //  void setTraficNuit(const LPTYTrafic pTrafic);

    // TODO in NMPB08 `mode calcul` is obsoleted. 
    //      Is now represented by the RoadFlowType at RoadTrafficComponent level.

    /*
     * Calcul du spectre de la route en prenant en compte sont trafic
     */
    TYSpectre computeSpectre(const LPTYTrafic regime);

    /**
     * Mise a jour des caracteristiques acoustiques de la route
     */
    virtual bool updateAcoustic(const bool& force = false);

    /**
     * \brief Required the road to update its altitude after altimetry changed
     *
     * \param alti the altimetry the altitude must be updated from
     * \param the GeoNode associated with this Road
     * \return whether the update succeeded.
     */
    virtual bool updateAltitudes(const TYAltimetrie& alti, LPTYRouteGeoNode pGeoNode );

    const RoadTrafficComponent& getRoadTrafficComponent(
        enum TrafficRegimes regime, enum RoadVehicleType vehic_type) const;

private:
    virtual void distriSrcs()
    {assert(false && "You must use distriSrcs(const TYAltimetrie&) for roads");}

    /**
     * \brief internally set the RoadTraffic's array of RoadTrafficComponents
     *        according to the given regime.
     */
    void setRoadTrafficArrayForRegime(enum TrafficRegimes regime);

protected:

    RoadTrafficComponent& getRoadTrafficComponent(
        enum TrafficRegimes regime, enum RoadVehicleType vehic_type);

    /**
     * Calcul de la pente moyenne de la route
     */
    double calculPenteMoyenne();

    /**
     * Distribution des sources
     */
    virtual void distriSrcs(const TYAltimetrie& alti, LPTYRouteGeoNode pGeoNode);

    /// Get/Set de l'offset des sources de la route
    double getOffSet() { return _offSet; }
    void setOffSet(const double& offSet) { _offSet = offSet; }

    // Membres
protected:

    RoadTraffic road_traffic;
    TYTrafic traffic_regimes[NB_TRAFFIC_REGIMES];

    ///Vitesse Moyenne
    double _vitMoy; // TODO  ongoing replacement
    ///Trafic de jour
    LPTYTrafic _pTraficJour; // TODO  ongoing replacement
    ///Trafic de nuit
    LPTYTrafic _pTraficNuit; // TODO  ongoing replacement
    ///Spectre du bruit routier normalise pondere A (utilise par compute spectre)


    // Hauteur des sources par rapport a la route
    double _offSet;
};

#endif // __TY_ROUTE__
