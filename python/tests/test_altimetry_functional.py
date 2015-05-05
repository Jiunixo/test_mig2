"""Functional test cases for Code_TYMPAN altimetry."""

import os.path
import unittest

from utils import TEST_PROBLEM_DIR

from tympan.models.project import Project
from tympan.altimetry import AltimetryMesh


class AltimetryFunctionalTC(unittest.TestCase):
    """Functional test cases for Tympan altimetry"""

    debug = False    # activate Tympan lib logs
    saveply = False  # save PLY file with mesh for visualisation

    @unittest.expectedFailure
    def test_les_barthes(self):
        fname = 'Tympan_Les_Barthes_avec_vegetation_Pb.xml'
        altim = self.build_altimetry(fname)
        coords = (10, 10)
        matid = '{4e550506-88c8-4985-abc2-47d221235787}'
        altitude_bounds = (230, 240)
        self.check_altitude_at(altim, coords, altitude_bounds)
        self.check_material_at(altim, coords, matid)

    def test_alti_pente_cste(self):
        fname = 'Site_pente_constante.xml'
        altim = self.build_altimetry(fname)
        # Test sur un point quelquonque
        coords = (-125, 10)
        altitude_bounds = (15, 15)
        self.check_altitude_at(altim, coords, altitude_bounds)
        # Test sur un point d'une courbe de niveau
        coords = (0, -30)
        altitude_bounds = (40, 40)
        self.check_altitude_at(altim, coords, altitude_bounds)

    def build_altimetry(self, fname):
        """Return the AltimetryMesh instance from Tympan XML file name"""
        fpath = os.path.join(TEST_PROBLEM_DIR, fname)
        project = Project.from_xml(fpath, verbose=self.debug)
        altim = AltimetryMesh.from_site(project.site)
        site = altim.equivalent_site
        if self.saveply:
            plyfile =  os.path.abspath(fname.split('.')[0] + '.ply')
            altim.to_ply(plyfile)
            print 'PLY file saved at ', plyfile
        return altim

    def assertWithin(self, value, lower, upper):
        """Check that `lower <= value <= upper`"""
        if value < lower:
            self.fail('%s is below lower bound %s' (value, lower))
        if value > upper:
            self.fail('%s is above upper bound %s' (value, upper))

    def check_altitude_at(self, altim, coords, altitude_bounds):
        """Check that altitude of a point located at `coords=(x, y)` in the
        altimetry mesh is withing `altitude_bounds=(lower, upper)`.

        `altim` parameter should be an instance of `AltimetryMesh`.
        """
        altitude = altim.mesh.point_altitude(coords)
        self.assertWithin(altitude, *altitude_bounds)

    def check_material_at(self, altim, coords, expected_material_id=None):
        """Check that material found at `coords=(x, y)` matchs with
        `expected_material_id`. `expected_material_id` may be None, if the
        default material is expected at this coordinate.

        `altim` parameter should be an instance of `AltimetryMesh`.
        """
        # Compute face handle (or vertex) corresponding to coordinates.
        fh, vh_or_i = altim.mesh.locate_point(coords)
        if expected_material_id is None:
            self.assertIsNotNone(altim.material_by_face[fh])
        else:
            self.assertEqual(altim.material_by_face[fh].id,
                             expected_material_id)


if __name__ == '__main__':
    unittest.main()
