"""solver models for Code_TYMPAN, headers
"""
from libcpp.string cimport string

from tympan.models.common cimport OPoint3D, OSpectre, OSpectreComplex
from tympan.core cimport shared_ptr
from libcpp.vector cimport vector

cdef class ProblemModel:
    cdef AcousticProblemModel* thisptr

cdef class ResultModel:
    cdef AcousticResultModel* thisptr


cdef extern from "Tympan/models/solver/acoustic_problem_model.hpp" namespace "tympan":
    cdef cppclass AcousticProblemModel:
        size_t npoints()
        size_t ntriangles()
        size_t nmaterials()
        size_t nsources()
        size_t nreceptors()
        AcousticTriangle &triangle(size_t tri_idx)
        AcousticSource& source(size_t idx)
        AcousticReceptor& receptor(size_t idx)
        OPoint3D& node(size_t idx)
        shared_ptr[AcousticMaterialBase] make_material(const string& name, double resistivity)
        shared_ptr[AcousticMaterialBase] make_material(const string& name, const OSpectreComplex& spectrum)
        size_t make_triangle(size_t n1, size_t n2, size_t n3)
        size_t make_node(const OPoint3D&)
        size_t make_source(const OPoint3D& point_, const OSpectre& spectrum_)
        size_t make_receptor(const OPoint3D& point_)

cdef extern from "Tympan/models/solver/acoustic_result_model.hpp" namespace "tympan":
    cdef cppclass SpectrumMatrix:
        const vector[OSpectre]& by_receptor(size_t receptor_idx) const
        const OSpectre& element "operator()"(size_t receptor_idx, size_t sources_idx)
        void resize(size_t nb_receptors, size_t nb_sources)
        size_t nb_sources() const
        size_t nb_receptors() const
        void clearReceptor(size_t receptor_idx)

    cdef cppclass AcousticResultModel:
        SpectrumMatrix& get_data()

cdef extern from "Tympan/models/solver/data_model_common.hpp":
    cdef cppclass BaseEntity:
        pass

cdef extern from "Tympan/models/solver/entities.hpp" namespace "tympan":
    cdef cppclass AcousticSource(BaseEntity):
        OPoint3D position
        OSpectre spectrum

    cdef cppclass AcousticReceptor(BaseEntity):
        OPoint3D position

    cdef cppclass AcousticTriangle:
        shared_ptr[AcousticMaterialBase] made_of
        size_t n[3]

    cdef cppclass AcousticMaterialBase:
        pass

    cdef cppclass AcousticBuildingMaterial(AcousticMaterialBase):
        AcousticBuildingMaterial(const string& name_, const OSpectreComplex& spectrum)

    cdef cppclass AcousticGroundMaterial(AcousticMaterialBase):
        AcousticGroundMaterial(const string& name_, double resistivity_)

    cdef cppclass SourceDirectivityInterface:
        pass

    cdef cppclass SphericalSourceDirectivity(BaseEntity, SourceDirectivityInterface):
        pass

    cdef cppclass CommonFaceDirectivity(BaseEntity, SourceDirectivityInterface):
        pass

    cdef cppclass VolumeFaceDirectivity(CommonFaceDirectivity):
        pass

    cdef cppclass ChimneyFaceDirectivity(CommonFaceDirectivity):
        pass

    cdef cppclass BaffledFaceDirectivity(CommonFaceDirectivity):
        pass


cdef acousticproblemmodel2problemmodel(AcousticProblemModel* apm)
cdef acousticresultmodel2resultmodel(AcousticResultModel* arm)