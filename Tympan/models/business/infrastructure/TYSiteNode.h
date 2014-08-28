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

#ifndef __TY_SITENODE__
#define __TY_SITENODE__

#include "Tympan/models/business/infrastructure/TYTopographie.h"
#include "Tympan/models/business/infrastructure/TYInfrastructure.h"
#include "Tympan/models/business/TYCalcul.h"

/// Systeme SIG
typedef enum {TYMPAN, LAMBERTIIe, GPS} systemSIG;


///Noeud geometrique de type TYSiteNode.
typedef TYGeometryNode TYSiteNodeGeoNode;
///Smart Pointer sur TYSiteNodeGeoNode.
typedef SmartPtr<TYSiteNodeGeoNode> LPTYSiteNodeGeoNode;
///Collection de noeuds geometriques de type TYSiteNode.
typedef std::vector<LPTYSiteNodeGeoNode> TYTabSiteNodeGeoNode;

class TYSiteNode: public TYElement
{
    OPROTODECL(TYSiteNode)
    TY_EXTENSION_DECL_ONLY(TYSiteNode)
    TY_EXT_GRAPHIC_DECL_ONLY(TYSiteNode)

    /**
     * Constructeur.
     */
    TYSiteNode();
    /**
     * Constructeur par copie.
     */
    TYSiteNode(const TYSiteNode& other);
    /**
     * Destructeur.
     */
    virtual ~TYSiteNode();

    ///Operateur =.
    virtual TYSiteNode& operator=(const TYSiteNode& other);
    ///Operateur ==.
    virtual bool operator==(const TYSiteNode& other) const;
    ///Operateur !=.
    virtual bool operator!=(const TYSiteNode& other) const;

    virtual bool deepCopy(const TYElement* pOther, bool copyId = true);

    virtual std::string toString() const;

    virtual DOM_Element toXML(DOM_Element& domElement);
    virtual int fromXML(DOM_Element domElement);

    virtual void getChilds(LPTYElementArray& childs, bool recursif = true);

    virtual void setIsGeometryModified(bool isModified);

    /**
     * Vide toutes les listes d'elements composants cet objet.
     */
    void purge() {  _listSiteNode.clear(); }

    /// Accesseurs
    /**
     * Get/Set du projet parent
     */
    TYProjet* getProjet() { return _pProjet; }
    const TYProjet* getProjet() const { return _pProjet; }

    /**
     * Get/Set du projet parent
     */
    void setProjet(TYProjet* pProjet);

    /**
     * Set/Get de la prise en compte de l'emprise comme courbe de niveau.
     */
    bool getUseEmpriseAsCrbNiv() const { return _bEmpriseAsCrbNiv; }
    /**
     * Set/Get de la prise en compte de l'emprise comme courbe de niveau.
     */
    void setUseEmpriseAsCrbNiv(bool b) { _bEmpriseAsCrbNiv = b; }

    /// Get/Set de l'altitude associee a la courbe de niveau
    void setAltiEmprise(const double& altiEmprise) { _altiEmprise = altiEmprise; }
    const double getAltiEmprise() const { return _altiEmprise; }

    /**
     * Set/Get de la topographie.
     */
    LPTYTopographie getTopographie() { return _pTopographie; }
    /**
     * Set/Get de la topographie.
     */
    const LPTYTopographie getTopographie() const { return _pTopographie; }

    /**
     * Set/Get de la topographie.
     */
    void setTopographie(const LPTYTopographie pTopo)
    {
        _pTopographie = pTopo;
        _pTopographie->setParent(this);
        setIsGeometryModified(true);
    }

    /**
     * Set/Get de l'ensemble des infrastructures.
     */
    LPTYInfrastructure getInfrastructure() { return _pInfrastructure; }
    /**
     * Set/Get de l'ensemble des infrastructures.
     */
    const LPTYInfrastructure getInfrastructure() const { return _pInfrastructure; }
    /**
     * Set/Get de l'ensemble des infrastructures.
     */
    void setInfrastructure(const LPTYInfrastructure& pInfra)
    {
        _pInfrastructure = pInfra;
        _pInfrastructure->setParent(this);
        setIsGeometryModified(true);
    }

    /**
     * Set/Get du flag d'utilisation d'une image de fond.
     */
    bool getUseTopoFile() const { return _useTopoFile; }
    /**
     * Set/Get du flag d'utilisation d'une image de fond.
     */
    void setUseTopoFile(bool flag) { _useTopoFile = flag; }

    /**
     * Set/Get du fichier de topographie temporaire.
     */
    std::string getTopoFile() const { return _topoFile; }
    /**
     * Set/Get du fichier de topographie temporaire.
     */
    void setTopoFile(std::string fileName) { _topoFile = fileName; }
    /**
     * Set/Get de l'extension du fichier de topographie.
     */
    std::string getTopoFileExtension() const { return _topoFileExtension; }
    /**
     * Set/Get de l'extension du fichier de topographie.
     */
    void setTopoFileExtension(std::string ext) { _topoFileExtension = ext; }
    /**
     * Set/Get du nom du fichier de topographie.
     */
    std::string getTopoFileName() const { return _topoFileName; }
    /**
     * Set/Get du nom du fichier de topographie.
     */
    void setTopoFileName(std::string name) { _topoFileName = name; }
    /**
     * Set/Get du chemin du fichier image temporaire.
     */
    static const std::string& getTopoFilePath();
    /**
     * Set/Get du chemin du fichier image temporaire.
     */
    static void setTopoFilePath(const std::string& path);

    /**
     * Mise a jour du tableau des pixels de l'image de fond a partir d'un fichier.
     *
     * @param fileName Le nom du fichier contenant l'image.
     */
    void loadTopoFile(std::string fileName);
    void loadTopoFile();

    /**
     * Set/Get de l'echelle.
     */
    float getEchelle() const { return _echelle; }
    /**
     * Set/Get de l'echelle.
     */
    void setEchelle(float e) { _echelle = e; setIsGeometryModified(true); }

    /**
     * Set/Get de l'orientation.
     */
    TYSegment getOrientation() const { return _orientation; }
    /**
     * Set/Get de l'orientation.
     */
    void setOrientation(TYSegment seg) { _orientation = seg; setIsGeometryModified(true); }

    /**
     * Set/Get de la position.
     */
    TYPoint getPosition() const { return _position; }

    /**
     * Set/Get du test de site racine.
     */
    bool getRoot() const { return _root; }

    /**
     * Set/Get du test de site racine.
     */
    void setRoot(bool b) { _root = b; }

    /**
     * Set/Get de la position.
     */
    void setPosition(TYPoint pt) { _position = pt; setIsGeometryModified(true); }

    /**
     * Set/Get de la liste des sites.
     */
    TYTabSiteNodeGeoNode& getListSiteNode() { return _listSiteNode; }

    /**
     * Set/Get de la liste des sites.
     */
    const TYTabSiteNodeGeoNode& getListSiteNode() const { return _listSiteNode; }

    /**
     * Ajoute un site a la liste des sites.
     *
     * @param site Element a ajouter.
     */
    bool addSiteNode(LPTYSiteNodeGeoNode pSiteNodeGeoNode);

    /**
     * Ajoute un site a la liste des sites.
     *
     * @param site Element a ajouter.
     */
    bool addSiteNode(LPTYSiteNode pSiteNode);

    /**
     * Supprime une entree a la liste des site nodes.
     */
    bool remSiteNode(const LPTYSiteNodeGeoNode pSiteNodeGeoNode);

    /**
     * Supprime une entree a la liste des site nodes.
     */
    bool remSiteNode(const LPTYSiteNode pSiteNode);

    /**
     * Supprime une entree a la liste des site nodes
     * a partir de son identifiant.
     */
    bool remSiteNode(QString idSiteNode);

    /**
     * Retrouve le GeoNode associe a un site node.
     *
     * @param pSiteNode L'element a chercher.
     *
     * @return Le GeoNode associe a l'element a chercher si trouve sinon NULL.
     */
    LPTYSiteNodeGeoNode findSiteNode(const LPTYSiteNode pSiteNode);

    /**
     * Set/Get de la liste des sites.
     */
    void setListSiteNode(const TYTabSiteNodeGeoNode& list) { _listSiteNode = list; setIsGeometryModified(true); }

    /**
     * Initialise l'état "isInCurrentCalcul" des enfants d'un projet
     */
    virtual void setChildsNotInCurrentCalcul();

    /**
     * Collecte l'ensemble des sites de ce site.
     * Ce site peut etre inclu et l'appel peut etre recursif.
     *
     * @param include Pour inclure ce site.
     *
     * @return Une collection de site remplie avec les bons changements de repere.
     */
    TYTabSiteNodeGeoNode collectSites(bool include = true) const;

    //3. -ELEMENT PROPRE AU CALCUL
    /**
     * Merge : construit un nouveau site compose de tous les elements de
     * ce site et des sites enfants/imbriques en tenant compte des
     * changements de repere pour chacun d'eux.
     * Attention : une fois les sites merges le parent des composants dans
     * le site merge sera le site merge lui-meme, il peut donc etre necessaire
     * d'appeler la methode 'reparent()' pour utiliser les sites imbriques
     * convenablement.
     *
     * @return Un nouveau site equivalent aux sites imbriques "mis a plat".
     *
     * @see reparent()
     */
    LPTYSiteNode merge();

    /**
     * Reattribut le bon parent aux composants de facon recursive.
     *
     * @see merge()
     */
    virtual void reparent();

    /**
     * Verifie si cet element est present dans la liste d'ID passee.
     * S'il est present cela signifie qu'il est actif dans le
     * Calcul courant, c'est cet etat qui est retourne.
     *
     * @param listID La liste d'ID du Calcul courant.
     * @param recursif Indique si la mise a jour du Calcul courant doit etre recursive.
     *
     * @return <code>true</code> si cet objet est actif dans le Calcul concerne;
     *         <code>false</code> sinon.
     */
    virtual void updateCurrentCalcul(TYListID& listID, bool recursif = true);

    /**
     * Ajoute les elements du site au calcul
     */
    virtual bool addToCalcul();

    /**
     * Supprime tous les éléments d'un site du calcul
     */
    virtual bool remFromCalcul();

    /**
     * Donne atmosphere � utiliser pour topographie
     */
    virtual void setAtmosphere(const LPTYAtmosphere& pAtmosphere);

    /**
     * Precalcul du nombre d'onde dans le sol des que les caracteristiques de
     * l'atmosphere sont connus. Fonction executees au lancement du calcul.
     */
    void updateSol();

    /**
     * This method mainly calls \c do_updateAltimetrie and handle exceptions
     * be capturing them and returning a success status.
     *
     * \return weather the update succeeded
     */
    virtual bool updateAltimetrie(const bool& force = false);

    void uuid2tysol(const std::deque<std::string>& material_ids, std::deque<LPTYSol>& materials);

protected:
    /**
     * Mise a jour de l'altimetrie.
     * Dans TYSite, cette methode appelle essentiellement 'computeAltimetrie' de
     * TYTopographie, mais elle peut etre surchargee pour des besoins specifiques
     * lors du calcul de l'altimetrie.
     */
    virtual void do_updateAltimetrie(const bool& force = false);

public:
    /*
     * Reads a mesh from 'filename' ply file, and puts it in 'points', 'triangles'
     * and 'materials'.
     */
    void readMesh(std::deque<OPoint3D>& points, std::deque<OTriangle>& triangles,
            std::deque<LPTYSol>& materials, const QString& filename);

    /**
     * Mise a jour de l'altitude pour les elements de l'infrastructure.
     * Le parametre force contraint l'altimetrisation des objets
     */
    virtual void updateAltiInfra(const bool& force = false);

    /**
     * Mise a jour de l'acoustique des elements presents dans le site
     */
    virtual void updateAcoustique(const bool& force = false);

    /**
     * Effectue en une seule operation la mise a jour de la topo et de l'infrastructure
     */
    void update(const bool& force = false);

    /**
     * Recherche le parent "métier" d'un élement et le met é jour
     * @param pElem Pointeur sur un objet de type TYElement
     */
    bool update(TYElement* pElem);


    /**
     * Collecte l'ensemble des points necessaires a la generation
     * de l'altimetrie pour les site imbriques.
     */
    // XXX The process is being rewritten and this function removed
    // virtual TYTabPoint collectPointsForAltimetrie() const;

    /**
     * Get du parametre de triangulation de Delaunay
     */
    double getDelaunay();

    /**
     * Cree la liste des faces des elements du site.
     *
     */
    void getListFaces(TYTabAcousticSurfaceGeoNode& tabFaces, unsigned int& nbFaceInfra, std::vector<bool>& EstUnIndexDeFaceEcran) const;
    void getListFacesWithoutFloor(TYTabAcousticSurfaceGeoNode& tabFaces, unsigned int& nbFaceInfra, std::vector<bool>& EstUnIndexDeFaceEcran, std::vector<std::pair<int, int> >& indices, std::vector<int>& etages) const;

    /*!
     * brief : Attempt to find the faces placed on the ground among the machines
     * and the buildings of the site
     * TYUUID: volume node id (volume nodes are buildings or machines)
     * deque<TYTabPoint3D>: series of points defining the contour of the face of
     *  the volumes that are on the ground in a global scale
     */
    void getFacesOnGround(std::map<TYUUID, TYTabPoint3D>& contours) const;

    /*!
     * brief: from a series of acoustic volumes (forming an acoustic volume node)
     * and a transform matrix allowing to convert them to a global scale,
     * return the points constituting the acoustic surface placed on the ground
     */
    void groundBasedFaces(const TYTabAcousticVolumeGeoNode& volumes,
                          const OMatrix& global_matrix,
                          std::map<TYUUID, TYTabPoint3D>& contours) const;

    /// Get/Set du choix du systeme de repere du SIG
    int getSIGType() { return _SIGType; }
    const int getSIGType() const { return _SIGType; }
    void setSIGType(int SIGType) { _SIGType = (systemSIG) SIGType; }

    /// Get/Set des coordonnees dans le repere du SIG
    double getSIG_X() { return _SIG_X; }
    const double getSIG_X() const { return _SIG_X; }
    void setSIG_X(const double& SIG_X) { _SIG_X = SIG_X; }

    double getSIG_Y() { return _SIG_Y; }
    const double getSIG_Y() const { return _SIG_Y; }
    void setSIG_Y(const double& SIG_Y) { _SIG_Y = SIG_Y; }

    /// Export au format csv sur un flux transmis
    virtual void exportCSV(std::ofstream& ofs);

    /* Check if the altimetry is up-to-date. This is checked only on the root sites
     */
    bool getAltimetryUpToDate(){ return _alti_uptodate; }

    /* Inform a root site that its altimetry is outdated.
     */
    void setAltimetryUpToDate(bool uptodate){ _alti_uptodate = uptodate; }

protected:
    /**
     * Methode interne utilisee par 'merge()' pour recopier les composants
     * d'un site vers un autre en appliquant le changement de repere defini
     * par la matrice passee.
     *
     * @param pSiteFrom Site a ajouter.
     * @param matrix Changement de repere a effectuer avant l'ajout.
     * @param pSiteTo Site auquel on ajoute le 1er site.
     */
    void appendSite(LPTYSiteNode pSiteFrom, const OMatrix& matrix, LPTYSiteNode pSiteTo);

    // Membres
private:
    ///Chemin du dossier image temporaire.
    static std::string* _topoFilePath;

protected:
    /// Projet auquel appartient (eventuellement) le site
    TYProjet* _pProjet;

    /// Utilisation de l'emprise comme courbe de niveau
    bool _bEmpriseAsCrbNiv;

    /// Altitude associee a l'emprise (s'il y a lieu)
    double _altiEmprise;

    ///Topographie.
    LPTYTopographie _pTopographie;

    ///Infrastructure.
    LPTYInfrastructure _pInfrastructure;

    ///Flag d'utilisation d'une image de fond.
    bool _useTopoFile;

    ///Nom du fichier de topographie.
    std::string _topoFileName;

    ///Nom du fichier de topographie temporaire.
    std::string _topoFile;

    ///Extension du fichier de topographie.
    std::string _topoFileExtension;

    ///Echelle du site.
    float _echelle;
    ///Orientation du Nord.
    TYSegment _orientation;
    ///Position.
    TYPoint _position;

    /// Nombre de faces de l'infrastructure (machines, bâtiments, etc ...)
    unsigned int _nbFaceInfra;

    unsigned int _nbFaces; // Nombre de faces total

    ///Test si site racine.
    bool _root;

    /// Coordonnees SIG
    systemSIG _SIGType;

    double _SIG_X;
    double _SIG_Y;

    ///Liste des sites.
    TYTabSiteNodeGeoNode _listSiteNode;

    ///Flag de modification de l'image de fond.
    bool _isTopoFileModified;

    // Indicates whether the altimetry is up-to-date on this site (this value
    // is checked only on the root sites)
    bool _alti_uptodate;

};



#endif // __TY_SITENODE__
