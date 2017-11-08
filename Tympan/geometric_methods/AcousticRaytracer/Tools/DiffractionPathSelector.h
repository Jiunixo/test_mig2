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

#ifndef DIFFRACTION_PATH_SELECTOR
#define DIFFRACTION_PATH_SELECTOR

#include "Selector.h"

/*!
 * \brief : To invalid rays with path length difference (produced by diffraction effect) greater than a value
 */
template<typename T>
class DiffractionPathSelector : public Selector<T>
{
public :
	/// Constructor
    DiffractionPathSelector(double _maximumDelta = 8) : Selector<T>() { maximumDelta = _maximumDelta; }
    virtual Selector<T>* Copy()
    {
        DiffractionPathSelector* newSelector = new DiffractionPathSelector(maximumDelta);
        newSelector->setIsDeletable(this->deletable);
        return newSelector;
    }

    virtual SELECTOR_RESPOND canBeInserted(T* r, unsigned long long& replace)
    {
        vector< std::shared_ptr<Event> > *events = r->getEvents();
		if (events->size() == 0) { return SELECTOR_ACCEPT; }


		decimal cumul_delta = 0.;
		decimal cumul_distance = 0.;
		bool notLast = true;
		Recepteur *pRecep = static_cast<Recepteur*>( r->getRecepteur() );
		Source *pSource = r->getSource();

		vec3 origin = pRecep->getPosition();
		vec3 current_pos = origin;

		vector< std::shared_ptr<Event> >::reverse_iterator rit = events->rbegin();
		while(rit != events->rend())
		{
			cumul_distance += (*rit)->getPosition().distance(current_pos);
			if ( (*rit)->getType() == SPECULARREFLEXION ) 
			{
				cumul_delta += ( cumul_distance - origin.distance( (*rit)->getPosition() ) );
				cumul_distance = 0;
				origin = (*rit)->getPosition();
				notLast = false;
			}
			else
			{
				notLast = true;
			}

			current_pos = (*rit)->getPosition(); 
			rit ++;
		};

		if (notLast) 
		{ 
			cumul_distance += current_pos.distance( pSource->getPosition() );
			cumul_delta += ( cumul_distance - origin.distance( pSource->getPosition() ) );
		}
 
        if ( cumul_delta > maximumDelta )
        {
			return SELECTOR_REJECT;
        }

        return SELECTOR_ACCEPT;
    }
    /// Select the ray
    virtual void insert(T* r, unsigned long long& replace) { return; }

    virtual bool insertWithTest(T* r)
    {
        vector< std::shared_ptr<Event> > *events = r->getEvents();
		
		if (events->size() == 0) { return true; }

		decimal cumul_delta = 0.;
		decimal cumul_distance = 0.;
		bool notLast = true;
		
		Recepteur *pRecep = static_cast<Recepteur*>( r->getRecepteur() );
		Source *pSource = r->getSource();

		vec3 origin = pRecep->getPosition();
		vec3 current_pos = origin;

		vector< std::shared_ptr<Event> >::reverse_iterator rit = events->rbegin();
		while(rit != events->rend())
		{
			cumul_distance += (*rit)->getPosition().distance(current_pos);
			if ( (*rit)->getType() == SPECULARREFLEXION ) 
			{
				cumul_delta += ( cumul_distance - origin.distance( (*rit)->getPosition() ) );
				cumul_distance = 0;
				origin = (*rit)->getPosition();
				notLast = false;
			}
			else
			{
				notLast = true;
			}

			current_pos = (*rit)->getPosition(); 
			rit ++;
		};

		if (notLast) 
		{ 
			cumul_distance += current_pos.distance( pSource->getPosition() );
			cumul_delta += ( cumul_distance - origin.distance( pSource->getPosition() ) );
		}
 
        if ( cumul_delta > maximumDelta )
        {
			return false;
        }

        return true;
    }
    /// Get maximumDelta
    double getMaximumDelta() { return maximumDelta; }
    /// Set maximumDelta
    void setMaximumDelta(double _maximumDelta) { this->maximumDelta = _maximumDelta; }


protected:
    double maximumDelta; //!< Maximal path length difference between rays produced by diffraction
};

#endif // DIFFRACTION_PATH_SELECTOR
