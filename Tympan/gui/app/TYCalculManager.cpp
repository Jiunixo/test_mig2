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

/**
 * \file TYCalculManager.cpp
 * \brief Gestionnaire des calculs acoustiques. Il fait l'interface entre l'IHM et le gestionnaire de donnees
 * pour la partie calcul
 */


#include <qcursor.h>
#include <qmessagebox.h>
#include <QTemporaryFile>

#include "Tympan/core/config.h"
#include "Tympan/core/chrono.h"
#include "Tympan/core/defines.h"
#include "Tympan/models/business/OLocalizator.h"
#include "Tympan/models/business/xml_project_util.h"
#include "Tympan/gui/app/TYApplication.h"
#include "Tympan/gui/app/EnvironmentUtils.h"
#include "Tympan/gui/app/TYProjetFrame.h"
#include "Tympan/gui/app/TYMessageManager.h"
#include "Tympan/gui/app/TYMainWindow.h"
#include "TYCalculManager.h"


#define TR(id) OLocalizator::getString("TYCalculManager", (id))
#define COMPUTATION_TIMEOUT 10000 // In ms

using namespace tympan;

TYCalculManager::TYCalculManager()
{
    _pCurrentCalcul = NULL;
}

TYCalculManager::~TYCalculManager()
{
    _pCurrentCalcul = NULL;
}

void TYCalculManager::setCurrent(LPTYCalcul pCalcul)
{
    _pCurrentCalcul = pCalcul;
}

bool TYCalculManager::launchCurrent()
{
    return launch(_pCurrentCalcul);
}

bool TYCalculManager::launch(LPTYCalcul pCalcul)
{
    if (!pCalcul)
    {
        return false;
    }

    TYProjet *pProject = pCalcul->getProjet();
    OMessageManager& logger =  *OMessageManager::get();
    // Start chrono
    OChronoTime startTime;

    if (!getTYApp()->_usePython)
    {
        logger.warning("Legacy computation (without Python)");
        LPTYSiteNode pSite = pProject->getSite();
        pSite->getTopographie()->sortTerrainsBySurface();
        pSite->updateAltiInfra(true);
        pSite->updateAcoustique(true);
        pProject->updateAltiRecepteurs(pSite->getTopographie()->getAltimetrie());
        pSite->setAtmosphere(pCalcul->getAtmosphere());
        pCalcul->go();
        pCalcul->goPostprocessing();
    }
    else
    {
        logger.debug("Computation through Python script");
        // Temporary XML file to give the current acoustic problem to the python
        // script
        QTemporaryFile problemfile;
        if (!problemfile.open())
        {
            logger.error(
                    "Could not open temporary file to export current project. Computation won't be done");
            return false;
        }
        problemfile.close();

        // Serialize current project
        ostringstream msg;
        try
        {
            save_project(problemfile.fileName().toUtf8().data(), pProject);
        }
        catch(const tympan::invalid_data& exc)
        {
            msg << boost::diagnostic_information(exc);
            logger.error(
                    "Could not export current project. Computation won't be done");
            logger.debug(msg.str().c_str());
            return false;
        }

        QTemporaryFile resultfile;
        if (!resultfile.open())
        {
            logger.error(
                    "Could not open temporary file to retrieve computation results.");
            return false;
        }
        resultfile.close();

        QStringList appli_env(QProcess::systemEnvironment());
        int tympan_debug_idx = appli_env.indexOf(QRegExp("^TYMPAN_DEBUG=(.*)"));
        if (tympan_debug_idx >= 0)
        {
            QString debug_option = appli_env[tympan_debug_idx].split('=')[1];
            if (debug_option.contains("xml", Qt::CaseInsensitive))// == "xml")
            {
                // If we are in debug mode, don't remove the temporary files once python
                // script is done
                problemfile.setAutoRemove(false);
                resultfile.setAutoRemove(false);
                logger.debug(
                        "The computation will run in debug mode. XML temporary files won't be removed afterwards. Input file: %s  Output file: %s.",
                        problemfile.fileName().toStdString().c_str(),
                        resultfile.fileName().toStdString().c_str());
            }
        }

        // Call python script "solve_project.py" with: the name of the file
        // containing the problem, the name of the file where to record
        // the result and the directory containing the solver plugin to use
        // to solve the acoustic problem
        QProcess python;
        QStringList args;
        QString absolute_plugins_path (QCoreApplication::applicationDirPath());
        absolute_plugins_path.append("/");
        absolute_plugins_path.append(PLUGINS_PATH);
        QString absolute_pyscript_path (QCoreApplication::applicationDirPath());
        absolute_pyscript_path.append("/");
        absolute_pyscript_path.append(PYSCRIPT);
        args << absolute_pyscript_path << problemfile.fileName() << resultfile.fileName()
            << absolute_plugins_path;

        logger.info(TR("id_msg_go_calcul"));

        // Deactivate GUI
        getTYMainWnd()->setEnabled(false);
        TYApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        float comp_duration (0.);
        bool comp_finished (false);
        // Send python script output to the current process std::out/err
        python.setProcessChannelMode(QProcess::ForwardedChannels);
        // Set PYTHONPATH to python subprocess
        QStringList env(python_qprocess_environment());
        python.setEnvironment(env);
        // Since scripts passed to QProcess are not launched through cmd.exe
        // under windows, we have to give QProcess the path to the python interpreter
        QString python_interp;
        try
        {
            python_interp = get_python_interp();
        }
        catch(const tympan::invalid_data& exc)
        {
            msg << boost::diagnostic_information(exc);
            logger.error(
                    "Could not find python interpreter to launch python script. Computation won't be done");
            logger.debug(msg.str().c_str());
            // Reactivate GUI
            TYApplication::restoreOverrideCursor();
            getTYMainWnd()->setEnabled(true);
            QMessageBox msgBox;
            msgBox.setText(
                    "L'interpreteur python n'a pas pu etre trouve.\nVeuillez verifier que la variable d'environnement TYMPAN_PYTHON_INTERP est correctement positionnee");
            msgBox.exec();
            return false;
        }
        python.start(python_interp, args);
        do
        {
            comp_finished = python.waitForFinished(COMPUTATION_TIMEOUT);
            if (comp_finished)
            {
                break;
            }
            if (python.error() == QProcess::Timedout)
            {
                //Computation still running
                comp_duration += (COMPUTATION_TIMEOUT/1000);
                logger.info("Computation still running after %.3f seconds", comp_duration);
                continue;
            }
            else
            {
                // Will have a bad exit status and will be handled below
                break;
            }
        }
        while(!comp_finished);

        int pystatus = python.exitStatus();
        if (pystatus == 1)
        {
            logger.error(
                    "QProcess running python script had a bad exit status. Error: %d. No results available.",
                    python.error());
            logger.info(TR("id_msg_calcul_failed"));
            // Reactivate GUI
            TYApplication::restoreOverrideCursor();
            getTYMainWnd()->setEnabled(true);
            QMessageBox msgBox;
            msgBox.setText("Le calcul a echoue, veuillez reessayer.");
            msgBox.exec();
            return false;
        }
        // Then read the result to update the internal model
        LPTYProjet result;
        try
        {
            result = load_project(resultfile.fileName().toUtf8().data());
        }
        catch(const tympan::invalid_data& exc)
        {
            msg << boost::diagnostic_information(exc);
            logger.error("Could not import computed project. No results available.");
            logger.debug(msg.str().c_str());
            // reactivate GUI
            TYApplication::restoreOverrideCursor();
            getTYMainWnd()->setEnabled(true);
            QMessageBox msgBox;
            msgBox.setText("Le fichier de resultats n'a pas pu etre lu.");
            msgBox.exec();
            return false;
        }

        // Update the current project with the results of the current acoustic
        // problem
        TYCalcul* pOldComp = pCalcul;
        pProject = result.getRealPointer();
        pCalcul = pProject->getCurrentCalcul();
        getTYApp()->getCurProjet()->setCurrentCalcul(pCalcul);
        // Can't remove current computation so first assign it and then remove
        // the previous one from the project
        getTYApp()->getCurProjet()->remCalcul(pOldComp);
        getTYMainWnd()->getProjetFrame()->setProjet(pProject);
    }

    // Compute and display computation time
    OChronoTime endTime;
    OChronoTime duration = endTime - startTime;
    unsigned long second  = duration.getTime() / 1000;
    unsigned long millisecond = duration.getTime() - second * 1000;
    logger.info("Temps de calcul : %02ld,%03ld sec.  (%ld msec.)", second,
            millisecond, duration.getTime());

    // Reactivate GUI
    TYApplication::restoreOverrideCursor();
    getTYMainWnd()->setEnabled(true);

    // Update graphics
    pCalcul->getParent()->updateGraphicTree();
    pCalcul->updateGraphicTree();
    getTYMainWnd()->updateModelers(false, false);
    TYElement::setIsSavedOk(true);

    // Update projet frame
    getTYMainWnd()->getProjetFrame()->setCalculDone(true);

    // Computation achieved with success
    logger.info(TR("id_msg_calcul_done"));

    return true;
}

bool TYCalculManager::askForResetResultat()
{
    bool ret = true;
    TYCalcul* pCalcul = NULL;
    TYProjet* pProjet = getTYApp()->getCurProjet().getRealPointer();
    if (pProjet)
    {
        pCalcul = pProjet->getCurrentCalcul();
    }
    else
    {
        return true;
    }

    if (pCalcul)
    {
        ret = true;
        getTYMainWnd()->getProjetFrame()->setCalculDone(false);

        /*// Question
        int del = QMessageBox::Yes;
        if (pCalcul->getIsAcousticModified())
        {
            del = QMessageBox::warning(getTYMainWnd(), getTYMainWnd()->windowTitle(), TR("id_msg_del_resultat"), QMessageBox::Yes, QMessageBox::No);
        }
        if (del == QMessageBox::Yes)
        {
            // Ok pour effacer les resultats (uniquement si c'est un resultat calcule);
            if (pCalcul->getState() == TYCalcul::Actif)
            {
            // Reset du resultat
            pCalcul->getResultat()->purge();
            for (unsigned int i=0 ; i<pProjet->getPointsControl().size() ; i++)
            {
                pProjet->getPointsControl()[i]->purge(pCalcul);
            }

                pCalcul->setIsAcousticModified(false); // La calcul est a jour
            }

            ret = true;
        }
        else
        {
            ret = false;
        }*/
    }

    return ret;
}

bool TYCalculManager::updateAcoustic(TYElement* pElement)
{
    bool ret = false;
    TYSiteNode* pSite = dynamic_cast<TYSiteNode*>(pElement);
    if (pSite != nullptr)
    {
        pSite->update();
    }
    LPTYAcousticVolumeNode pAccVolNode = dynamic_cast<TYAcousticVolumeNode*>(pElement);
    if (pAccVolNode._pObj != nullptr)
    {
            TYMessageManager::get()->info(TR("id_msg_go_updateacoustic"));

            TYApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

            // Calcul acoustique
            ret = pAccVolNode->updateAcoustic();

            if (ret)
            {
                TYMessageManager::get()->info(TR("id_msg_updateacoustic_done"));
            }
            else
            {
                TYMessageManager::get()->info(TR("id_msg_updateacoustic_failed"));
            }

            // MaJ graphique
            pAccVolNode->getGraphicObject()->update(true);
            getTYMainWnd()->updateModelers(false, false);

            TYApplication::restoreOverrideCursor();
    }
    else
    {
        LPTYAcousticLine pLine = dynamic_cast<TYAcousticLine*>(pElement);
        if (pLine._pObj != nullptr)
        {
            TYMessageManager::get()->info(TR("id_msg_go_updateacoustic"));

            TYApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

            // Calcul acoustique
            ret = pLine->updateAcoustic();

            if (ret)
            {
                TYMessageManager::get()->info(TR("id_msg_updateacoustic_done"));
            }
            else
            {
                TYMessageManager::get()->info(TR("id_msg_updateacoustic_failed"));
            }

            // MaJ graphique
            pLine->getGraphicObject()->update(true);
            getTYMainWnd()->updateModelers(false, false);

            TYApplication::restoreOverrideCursor();
        }
    }

    return ret;
}
