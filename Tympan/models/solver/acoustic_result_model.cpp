/**
* \file acoustic_result_model.cpp
* \brief Implementation for top-level of the acoustic result model
*
* \date april 2nd 2014
* \author Laura Médioni <laura.medioni@logilab.fr>
*/

#include "acoustic_result_model.hpp"


namespace tympan
{

    AcousticResultModel::~AcousticResultModel()
    {
    };

    std::unique_ptr<AcousticResultModel> make_AcousticResultModel()
    { return std::unique_ptr<AcousticResultModel>(new AcousticResultModel()); }

} // namespace tympan
