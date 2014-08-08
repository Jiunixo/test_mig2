/**
 * \file acoustic_result_model.hpp
 * \brief This file provides the top-level declaration for the acoustic
 * result model
 *
 * \date march 13 2014
 * \author Laura Médioni <laura.medioni@logilab.fr>
 */

#ifndef TYMPAN__ACOUSTIC_RESULT_MODEL_H__INCLUDED
#define TYMPAN__ACOUSTIC_RESULT_MODEL_H__INCLUDED

#include <cstddef>
#include <vector>
#include <memory>

#include "data_model_common.hpp"
#include "entities.hpp"

namespace tympan
{

class SpectrumMatrix
{
public:

    typedef std::vector<std::vector<Spectrum> > impl_matrix_t;

    SpectrumMatrix();
    SpectrumMatrix(size_t nb_receptors, size_t nb_sources);
    virtual ~SpectrumMatrix() {};

    size_t nb_sources()   const { return _nb_sources; };
    size_t nb_receptors() const { return data.size(); };

    const Spectrum& operator()(size_t receptor_idx, size_t sources_idx) const;
    Spectrum& operator()(size_t receptor_idx, size_t sources_idx);
    void setSpectre(size_t receptor_idx, size_t sources_idx, Spectrum spectrum);

    const std::vector<Spectrum>& by_receptor(size_t receptor_idx) const;

    void clearReceptor(size_t receptor_idx);

    void clear() {data.clear(); };

    void resize(size_t nb_receptors, size_t nb_sources);

protected:
    impl_matrix_t data;
private:
    size_t _nb_sources;

}; // class SpectrumMatrix


class AcousticResultModel
{
public:
    AcousticResultModel() {};

    virtual ~AcousticResultModel();

    SpectrumMatrix& get_data() { return acoustic_data; }

protected: // data members

    SpectrumMatrix acoustic_data;

};  // class AcousticResultModel

std::unique_ptr<AcousticResultModel> make_AcousticResultModel();


} // namespace tympan




#endif // TYMPAN__ACOUSTIC_RESULT_MODEL_H__INCLUDED
