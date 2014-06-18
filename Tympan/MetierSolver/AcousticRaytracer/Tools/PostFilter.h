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

#include <map>
#include <deque>
#include "Tympan/MetierSolver/AcousticRaytracer/Ray/Ray.h"

#ifndef POST_FILTER_H
#define POST_FILTER_H

//typedef std::map< signature, std::vector<Ray*> > families;
//typedef std::list<Event*> sequence;
//typedef std::map< sequence, std::vector<Ray*> > sequenceMap;
typedef std::map< signature, std::vector<Ray*> > families;
typedef std::list<Event*> sequence;
typedef std::map< sequence, std::vector<Ray*> > sequenceMap;


class postFilter : public Base
{
public:
//	postFilter(std::vector<Ray*> *tabRay) : _tabRay(tabRay){}
	postFilter(std::deque<Ray*> *tabRay) : _tabRay(tabRay){}
    ~postFilter() { _tabRay = NULL; }

    /*!
     * \fn unsigned int Process();
     * \brief apply a filter to the group of valid rays found by ray tracing
     * \return number of rays suppressed
     */
    virtual unsigned int Process() { return 0; }

    /*!
     * \fn unsigned int buildFamilies(families& mapFamilies)
     * \brief Group rays with the same signature (same source and receptor events of same kind in the same order
     * \return Number of families created
     */
    inline virtual unsigned int buildFamilies(families& mapFamilies, typeevent typeEv)
    {
        for (unsigned int i = 0; i < _tabRay->size(); i++)
        {
            mapFamilies[_tabRay->at(i)->getSignature(typeEv)].push_back(_tabRay->at(i));
        }

        return mapFamilies.size();
    }

    /*!
     * \fn sequenceCount buildSequenceCount(const signature &sig, std::vector<Ray*>, const typeevent& evType)
     * \brief Sort rays by events of type evType encountered along is path
     */
    inline virtual sequenceMap buildSequenceMap(const signature& sig, std::vector<Ray*> tabRay, const typeevent& evType)
    {
        sequenceMap seqMap;

        for (unsigned int i = 0; i < tabRay.size(); i++)
        {
            sequence seq = buildSequence(sig, tabRay.at(i), evType);
            seqMap[seq].push_back(tabRay.at(i));
        }

        return seqMap;
    }

    /*!
     * \fn decimal minimum_distance(decimal thickness, decimal d_R);
     * \brief compute minimum acceptable distance between two rays
     *        thickness is the thickness of a ray after a given distance
     *        d_R is the difference length between two rays
     */
    inline decimal minimum_distance(const decimal& thickness, const decimal& d_R) const
    {
        return sqrt(thickness * thickness + d_R * d_R);
    }

protected:
    /*!
     * \fn sequence buildSequence(const signature &sig, Ray* ray, const typeevent& evType)
     * \brief build the sequence of physical shape encountred by the ray
     */
    inline virtual sequence buildSequence(const signature& sig, Ray* ray, const typeevent& evType)
    {
        sequence res;

        for (unsigned int i = 0; i < ray->getNbEvents(); i++)
        {
            if (ray->getEvents()->at(i).data()->getType() == evType) { res.push_back(ray->getEvents()->at(i).data()); }
        }

        return res;
    }

    inline virtual void cleanTab(std::vector<Ray*>& tabRays, vector<Ray*>::iterator& iter)
    {
        delete(*iter);
        (*iter) = NULL;

        tabRays.erase(iter);
    }

protected:

	std::deque<Ray*> *_tabRay;
//	std::vector<Ray*> *_tabRay;
};

#endif //POST_FILTER_H
