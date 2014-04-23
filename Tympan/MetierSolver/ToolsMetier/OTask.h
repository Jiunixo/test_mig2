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

#ifndef __O_TASK__
#define __O_TASK__

#include "ThreadingSupport.h"
#include "Tympan/Tools/OSmartPtr.h"

/**
 * \file OTask.h
 * \class OTask
 * \brief T�che d'une collection de thread
 *
 * Cette classe abstraite encapsule une t�che primitive pouvant etre executee dans une collection de thread.
 * Il est necessaire de la surcharger en implementant la methode virtuelle pure OTask::main qui
 * definie la t�che a effectuer.
 * Une t�che est poussee dans la collection de thread avec la methode OThreadPool::push,
 * elle commence a s'effectuer des lors qu'un thread est disponible.
 * Une t�che ne peut pas etre annulee apres son execution.
 *
 * \author Projet_Tympan
 */
class OTask : public IRefCount, public QWaitCondition, public QMutex
{
public:
    // Constructeur par default
    OTask();

    // Destructeur : attend la fin de la t�che pour detruire
    virtual ~OTask();

    /**
     * \fn bool isRunning() const;
     * \brief Retourne true si la t�che est en cours d'execution, faux sinon
     */
    bool isRunning() const;

    /**
     * \fn bool isCompleted() const;
     * \brief Retourne true si la t�che est terminee, faux sinon
     */
    bool isCompleted() const;

    /**
     * \fn bool isCanceled() const;
     * \brief Retourne true si la t�che a ete annulee, faux sinon
     */
    bool isCanceled() const;

    /**
     * \fn virtual void main() = 0;
     * \brief Procedure principale de la t�che.
     * Cette methode virtuelle pure doit etre surchargee dans une classe derivee pour decrire la t�che a effectuer
     */
    virtual void main() = 0;

    /**
     * \fn void reset();
     * \brief Reset le statut de la t�che (_running=false et _completed=false)
     */
    void reset();

protected:
    /// Running flag.
    bool _running;

    /// Completed flag.
    bool _completed;

    /// Cancel flag.
    bool _canceled;

    friend class OSlaveThread;
    friend class OThreadPool;
};

///Smart Pointer sur OTask.
typedef SmartPtr<OTask> LPOTask;

#endif // __O_TASK__
