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
 */

#ifndef __TY_GEOMETRY_NODE__
#define __TY_GEOMETRY_NODE__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MSC_VER
#pragma warning( disable : 4284 )
#endif

#include "Tympan/models/business/TYElement.h"
#include "TYRepere.h"
#include "Tympan/models/common/3d.h"

/**
 * Template pour la gestion des noeuds de geometrie.
 * Un repere est associe a un element geometrique.
 * Une matrice mis a jour a chaque fois que le repere est
 * modifie permet de gagner du temps sur les calculs de
 * changement de repere.
 */
///Map de TYElement pour la gestion de la hierarchie des GeoNode:.
class TYGeometryNode;
typedef std::map<TYElement*, TYGeometryNode*> TYMapPtrGeoNode;
///Liste ordonnee de pointeurs de TYElement.
typedef std::list<TYGeometryNode*> TYListPtrGeoNode;

class TYGeometryNode: public TYElement
{
    OPROTODECL(TYGeometryNode)
public:
    /**
     * Constructeur par defaut.
     *
     */
    TYGeometryNode();
    /**
     * Constructeur.
     *
     * @param pElt Un element a associer a ce noeud.
     * @param pParent Le parent a la fois de l'element et du GeoNode.
     */
    TYGeometryNode(TYElement* pElt, TYElement* pParent = NULL);

    /**
     * Constructeur a partir d'un smart pointer sur un element.
     *
     * @param pElt Un element a associer a ce noeud (smart pointer).
     * @param pParent Le parent a la fois de l'element et du GeoNode.
     */
    TYGeometryNode(LPTYElement pElt, TYElement* pParent = NULL);

    /**
     * Constructeur.
     *
     * @param repere Le repere associe a l'element.
     * @param pElt Un element a associer a ce noeud.
     */
    TYGeometryNode(const TYRepere& repere, TYElement* pElt);

    /**
     * Constructeur.
     *
     * @param repere Le repere associe a l'element.
     * @param pElt Un element a associer a ce noeud (smart pointer).
     */
    TYGeometryNode(const TYRepere& repere, LPTYElement pElt);

    /**
     * Constructeur.
     *
     * @param pElt Un element a associer a ce noeud.
     * @param matrix La matrice associee a l'element.
     */
    TYGeometryNode(TYElement* pElt, const OMatrix& matrix);

    /**
     * Constructeur.
     *
     * @param pElt Un element a associer a ce noeud (smart pointer).
     * @param matrix La matrice associee a l'element.
     */
    TYGeometryNode(LPTYElement pElt, const OMatrix& matrix);

    /**
     * Constructeur de copie.
     *
     * @param other Un autre objet du meme type.
     */
    TYGeometryNode(const TYGeometryNode& other);

    /**
     * Destructeur.
     */
    virtual ~TYGeometryNode();

    virtual DOM_Element toXML(DOM_Element& domElement);
    virtual int fromXML(DOM_Element domElement);

    virtual void getChilds(LPTYElementArray& childs, bool recursif = true);

    /**
     * Get of the element.
     *
     * @return A real pointer on the element.
     */
    TYElement* getElement() const { return _pElement._pObj; }

    /**
     * Set de l'element (smart pointer).
     *
     * @param pElt Le nouvel element (smart pointer).
     */
    void setElement(LPTYElement pElt);

    /**
     * Set de l'element (pointeur classique).
     *
     * @param pElt Le nouvel element (pointeur classique).
     */
    void setElement(TYElement* pElt);

    /**
     * Supprime l'element si celui-ci existe.
     */
    void delElement();

    /**
     * Set/Get du repere.
     */
    const ORepere3D& getORepere3D() const
    { return _repere; }

    ORepere3D& getORepere3D()
    { return _repere; }

    void setRepere(const ORepere3D& repere) { _repere = repere; }

    /**
     * Set/Get de la matrice de changement de repere.
     */
    OMatrix getMatrix() const { return _repere.asMatrix(); }

    /**
     * Set/Get de la matrice de changement de repere.
     */
    void setMatrix(const OMatrix& matrix);

    /**
     * Operateur de copie.
     */
    TYGeometryNode& operator=(const TYGeometryNode& other);

    /**
     * Operateur de comparaison.
     */
    bool operator==(const TYGeometryNode& other) const;

    /**
     * Operateur de comparaison.
     */
    bool operator!=(const TYGeometryNode& other) const;

    virtual bool deepCopy(const TYElement* pOther, bool copyId = true);

    /**
     * Determine la matrice de passage du repere local vers le repere absolu.
     */
    OMatrix localToGlobal() const;

    /**
     * Determine la matrice de passage du repere absolu vers le repere local.
     */
    OMatrix globalToLocal() const;
    
    /**
     * @brief Set the position of the element
     */
    void setPosition(const OPoint3D& pos);
    
    /**
     * @brief Get the position of the element
     */
    OPoint3D position() { return _repere._origin; }

    /**
     * @brief Set the rotation angle along axis x, y & z represented as an OPoint3D
     */
    void setRotation(const OPoint3D& rot);

    /**
     * @brief Get the rotation angle along axis x, y & z represented as an OPoint3D
     */
    OPoint3D rotation(); 

    /**
     * @brief Get the height above the ground
     */
    double getHauteur() { return _hauteur; }

    /**
     * \brief Set the heigth above the ground
     */
    void setHauteur(const double& hauteur) { _hauteur = hauteur; }

#if TY_USE_IHM
    virtual LPTYElementGraphic getGraphicObject();
#endif // TY_USE_IHM

    static TYGeometryNode*  GetGeoNode(TYElement* pElement);
    TYGeometryNode*         GetGeoNodeParent() const;
    void                    GetGeoNodeParentList(TYListPtrGeoNode& GetGeoNodeParents);
    // Membres
protected:
    //Problematique d'obtention du geoNode d'un element
    //- Cette fonctionalite est necessaire pour former une hierarchie de geoNode, car celle-ci decoule de celle des objets (les 'pElement').
    //- Cette information de correspondance Element-GeoNode n'est malheureusement pas stockee/stockable cote Element,
    //car elle peut changer au cours du temps. D'ou l'interet d'une map.
    //Map permettant d'acceder rapidement au GeoNode connaissant l'element sur lequel il pointe,
    //pour obtenir rapidement le geoNode de l'element:
    static TYMapPtrGeoNode* _geoNodeMap;
    static TYMapPtrGeoNode* GetGeoNodeMap();
    //En theorie, il ne peut y avoir qu'un GeoNode pour un element; en pratique, ce n'est (temporairement) pas le cas:
    //voir operateur =, setElement,...et toutes les methodes ou figure _pElement.
    //Afin de ne pas perdre d'information de correspondance, on maintient une liste "de doublons" contenant tous les GeoNode
    //pointant sur un element deja reference par un autre geoNode de la map.
    static TYListPtrGeoNode* _geoNodeDoublonsList;
    static TYListPtrGeoNode* GetGeoNodeDoublonsList();

    //Enregistre this comme geoNode officiel de this->_pElement:
    void addToTheMap();
    //Supprime toute reference a this comme geoNode officiel de this->_pElement:
    void removeFromTheMap();

    ///L'instance de l'element geometrique.
    LPTYElement _pElement;

    ///Le repere definissant la position et l'orientation de l'element.
    TYRepere _repere;

    /// Hauteur de l'element par rapport au sol
    double _hauteur;
};

#if TY_USE_IHM
#include "Tympan/gui/gl/TYGeometryNodeGraphic.h"
#endif

typedef SmartPtr<TYGeometryNode> LPTYGeometryNode;

#endif //__TY_GEOMETRY_NODE__
