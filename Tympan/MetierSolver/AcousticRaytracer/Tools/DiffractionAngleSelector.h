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

#ifndef DIFFRACTION_ANGLE_SELECTOR
#define DIFFRACTION_ANGLE_SELECTOR

#include "Selector.h"
#include "Tympan/MetierSolver/CommonTools/OGeometrie.h"

template<typename T>
class DiffractionAngleSelector : public Selector<T>
{

public :
    DiffractionAngleSelector() : Selector<T>() {}
    virtual Selector<T>* Copy()
    {
        DiffractionAngleSelector* newSelector = new DiffractionAngleSelector();
        newSelector->setIsDeletable(this->deletable);
        return newSelector;
    }

    virtual SELECTOR_RESPOND canBeInserted(T* r, unsigned long long& replace)
    {
        vector<QSharedPointer<Event> >* events = r->getEvents();

        if ( (events->size() == 0) || (r->getDiff() == 0) ) { return SELECTOR_ACCEPT; }
		
		vec3 beginPos = r->getSource()->getPosition();
		vec3 currentPos, nextPos, N, N1, N2, W, From, To;

		Diffraction *pDiff = NULL;
		int sgn = 0;

		decimal F1 = 0., F2 = 0., T1 = 0., T2 = 0., FT = 0.;

		vector<QSharedPointer<Event> >::iterator iter = events->begin();
		do
		{
			if ( (*iter)->getType() != DIFFRACTION ) 
			{ 
				beginPos = (*iter)->getPosition();
				iter++; 
				continue; 
			}

			currentPos = (*iter)->getPosition();

			pDiff = dynamic_cast<Diffraction*>( (*iter).data() );

			N1 = dynamic_cast<Cylindre*>(pDiff->getShape())->getFirstShape()->getNormal();
			N2 = dynamic_cast<Cylindre*>(pDiff->getShape())->getSecondShape()->getNormal();
			
			From = (currentPos - beginPos);
			From.normalize();
			
			if ( (iter+1) != events->end() )
			{
				nextPos = (*(iter+1)).data()->getPosition();
			}
			else
			{
				nextPos = static_cast<Recepteur*> (r->getRecepteur())->getPosition();
			}

			To = (nextPos - currentPos);
			To.normalize();

			FT = From * To;

			if ( ( 1. - FT ) < EPSILON_4 ) { return SELECTOR_ACCEPT; } // Vecteur limite tangent au plan de propagation

			if ( FT < 0. ) { return SELECTOR_REJECT; }  // Le vecteur sortant est "oppose" au vecteur entrant

			F1 = From * N1;
			F2 = From * N2;

			if ( (F1 * F2) > 0.) { return SELECTOR_REJECT; } 

			T1 = To * N1;
			T2 = To * N2;


			if ( (F1 <= 0.) && ( (T1 > EPSILON_4 ) || ( (T2 - F2) > EPSILON_4 ) ) )
			{ 
				return SELECTOR_REJECT; 
			}

			if ( (F2 <= 0.) && ( ( T2 > EPSILON_4 ) || ( (T1 - F1) > EPSILON_4 ) ) )
			{ 
				return SELECTOR_REJECT; 
			}

			beginPos = currentPos;
			iter++;
		}
		while( iter != events->end() );

		return SELECTOR_ACCEPT;
    }

	virtual void insert(T* r, unsigned long long& replace) { return; }

    virtual bool insertWithTest(T* r)
    {
        vector<QSharedPointer<Event> >* events = r->getEvents();

        if ( (events->size() == 0) || (r->getDiff() == 0) ) { return true; }
		
		vec3 beginPos = r->getSource()->getPosition();
		vec3 currentPos, nextPos, N, N1, N2, W, From, To;

		Diffraction *pDiff = NULL;
		int sgn = 0;

		decimal F1 = 0., F2 = 0., T1 = 0., T2 = 0., FT = 0.;

		vector<QSharedPointer<Event> >::iterator iter = events->begin();
		do
		{
			if ( (*iter)->getType() != DIFFRACTION ) 
			{ 
				beginPos = (*iter)->getPosition();
				iter++; 
				continue; 
			}

			currentPos = (*iter)->getPosition();

			pDiff = dynamic_cast<Diffraction*>( (*iter).data() );

			N1 = dynamic_cast<Cylindre*>(pDiff->getShape())->getFirstShape()->getNormal();
			N2 = dynamic_cast<Cylindre*>(pDiff->getShape())->getSecondShape()->getNormal();
			
			From = (currentPos - beginPos);
			From.normalize();
			
			if ( (iter+1) != events->end() )
			{
				nextPos = (*(iter+1)).data()->getPosition();
			}
			else
			{
				nextPos = static_cast<Recepteur*> (r->getRecepteur())->getPosition();
			}

			To = (nextPos - currentPos);
			To.normalize();

			FT = From * To;

			if ( ( 1. - FT ) < EPSILON_4 ) { return true; } // Vecteur limite tangent au plan de propagation

			if ( FT < 0. ) { return false; }  // Le vecteur sortant est "oppose" au vecteur entrant

			F1 = From * N1;
			F2 = From * N2;

			if ( (F1 * F2) > 0.) { return false; } 

			T1 = To * N1;
			T2 = To * N2;


			if ( (F1 <= 0.) && ( (T1 > EPSILON_4 ) || ( (T2 - F2) > EPSILON_4 ) ) )
			{ 
				return false; 
			}

			if ( (F2 <= 0.) && ( ( T2 > EPSILON_4 ) || ( (T1 - F1) > EPSILON_4 ) ) )
			{ 
				return false; 
			}

			beginPos = currentPos;
			iter++;
		}
		while( iter != events->end() );

		return true;
	}
};

#endif //DIFFRACTION_ANGLE_SELECTOR
