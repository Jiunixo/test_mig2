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

#ifndef LENGTH_SELECTOR
#define LENGTH_SELECTOR

#include "Selector.h"

/*!
 * \brief : Rejects rays which have traveled a distance greater than a given length
 */
template<typename T>
class LengthSelector : public Selector<T>
{
public :
	/// Default constructor
    LengthSelector(double _maxLength = 2000, OPERATOR _op = LESS_OR_EQUAL) : Selector<T>() { maxLength = _maxLength; op = _op;}
    virtual Selector<T>* Copy()
    {
        LengthSelector* newSelector = new LengthSelector(maxLength);
        newSelector->setIsDeletable(this->deletable);
        newSelector->setOperator(op);
        return newSelector;
    }

    virtual SELECTOR_RESPOND canBeInserted(T* r, unsigned long long& replace)
    {
        r->computeLongueur();
        switch (op)
        {
            case LESS:
                if (r->getLongueur() >= maxLength)
                {
                    return SELECTOR_REJECT;
                }
                break;
            case LESS_OR_EQUAL:
                if (r->getLongueur() > maxLength)
                {
                    return SELECTOR_REJECT;
                }
                break;
            case EQUAL:
                if (r->getLongueur() != maxLength)
                {
                    return SELECTOR_REJECT;
                }
                break;
            case GREATER_OR_EQUAL:
                if (r->getLongueur() < maxLength)
                {
                    return SELECTOR_REJECT;
                }
                break;
            case GREATER:
                if (r->getLongueur() <= maxLength)
                {
                    return SELECTOR_REJECT;
                }
                break;
        }
        return SELECTOR_ACCEPT;
    }
    /// Keep the ray
    virtual void insert(T* r, unsigned long long& replace) { return; }

    virtual bool insertWithTest(T* r)
    {
        r->computeLongueur();
        switch (op)
        {
            case LESS:
                if (r->getLongueur() >= maxLength)
                {
                    return false;
                }
                break;
            case LESS_OR_EQUAL:
                if (r->getLongueur() > maxLength)
                {
                    return false;
                }
                break;
            case EQUAL:
                if (r->getLongueur() != maxLength)
                {
                    return false;
                }
                break;
            case GREATER_OR_EQUAL:
                if (r->getLongueur() < maxLength)
                {
                    return false;
                }
                break;
            case GREATER:
                if (r->getLongueur() <= maxLength)
                {
                    return false;
                }
                break;
        }
        return true;
    }
    /**
     * \brief Get the maximal length
     */ 
    double getMaximumLength() { return maxLength; }
 
    /**
    * \brief Set the maximal length
    */ 
    void setMaximumLength(double _maximumLength) { this->maximumLength = _maximumLength; }

    /**
    * \brief Get the OPERATOR of this Selector
    */ 
    OPERATOR getOperator() { return op; }
 
    /**
    * \brief Set the OPERATOR of this Selector
    */ 
    void setOperator(OPERATOR _op) { op = _op; }

	/**
	* \brief Return the class type of the selector
	*/
	virtual const char* getSelectorName(){
		return typeid(this).name();
	}

protected:
    double maxLength;	//!< Maximal length criteria
    OPERATOR op;		//!< OPERATOR used (by default LESS_OR_EQUAL, which means this Selector keeps only ray with travelled distance<=maximalLength)

};

#endif
