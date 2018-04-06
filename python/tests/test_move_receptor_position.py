import unittest
import numpy as np
import sys
import os

from utils import TympanTC
from tympan.models.project import Project
from _util import compare_project_results, compare_txt_results, TOOLBOX_DATA_DIR
from move_receptor_position import main, get_rec_spec, import_xyz_csv


class Test(TympanTC):

    def test_compare_results(self):
        """ Test compare_results utility """
        project = Project.from_xml("Recepteur_mobile.xml")
        expected_project = Project.from_xml("Recepteur_mobile.xml")
        compare_project_results(project, expected_project, self)

    def test_compare_txt_results(self):
        """ Test compare_txt_results utility """
        positions = np.asarray([[2.0, 0.0], [0.0, 2.0], [-2.0, 0.0], [0.0, -2.0]])
        np.savetxt("actual.csv", positions, delimiter=',')
        # Should be ok:
        compare_txt_results("actual.csv", "actual.csv")
        # Should be ko:
        positions = np.asarray([[2.0, 0.001], [0.0, 2.0], [-2.0, 0.0], [0.0, -2.0]])
        np.savetxt("different.csv", positions, delimiter=',')
        try:
            compare_txt_results("actual.csv", "different.csv")
        except:
            print("OK, it detects expected differences.")
        else:
            sys.exit(-1)

    def test_non_regression(self):
        """ Check the result compared to expected one """
        project = main("Recepteur_mobile.xml", "", "")
        compare_txt_results("Niveau_sonore_moyen.txt", "Reference.txt")
        self.assertEqual(len(project.user_receptors), 5)

    def test_import_xyz_csv(self):
        """ Test CSV import """
        positions = np.asarray([[2.0, 0.0, 2.0], [0.0, 2.0, 2.1], [-2.0, 0.0, 2.2], [0.0, -2.0, 2.3]])
        # Write
        np.savetxt("comma.csv", positions, delimiter=',')
        # Read
        project = Project.from_xml("Recepteur_mobile.xml")
        x1, y1, z1 = import_xyz_csv("comma.csv", project)
        # Compare
        np.testing.assert_array_equal(x1, positions[:, 0])
        np.testing.assert_array_equal(y1, positions[:, 1])
        np.testing.assert_array_equal(z1, positions[:, 2])
        # Check if error if the point is well detected outside the domain:
        positions = np.asarray([[2.0, 10000.0, 2.0]])
        np.savetxt("comma.csv", positions, delimiter=',')
        try:
            import_xyz_csv("comma.csv", project)
        except:
            print("OK, it detects expected error.")
        else:
            sys.exit(-1)

    def test_add_receptor_from_csv_file(self):
        """ Test adding receptor from CSV file """
        # Recepteurs.csv contains the same 5 receptors than Recepteur_mobile.xml
        # so the result (average) should be the same than before with 10 receptors
        project = main("Recepteur_mobile.xml", "Recepteurs.csv", "")
        compare_txt_results("Niveau_sonore_moyen.txt", "Reference.txt")
        # Check there is 10 receptors now:
        self.assertEqual(len(project.user_receptors), 10)

    def test_global(self):
        """ Global test of move_receptor_position.py """
        # Four receptors around the source in a test_global.csv file
        distance = 50
        positions = np.asarray([[distance, 0.0, 2.0],
                                [0.0, distance, 2.0],
                                [-distance, 0.0, 2.0],
                                [0.0, -distance, 2.0]])
        np.savetxt("test_global.csv", positions, delimiter=',')

        # Run the main fonction of the module
        # It reads the project test_global.xml,
        # import the receptors from CSV file
        # export the results into TXT file
        project = main("test_global.xml", "test_global.csv", "receptor_in_circle.xml", "test_global.txt")

        # We want to check the average is the same than on each receptor
        # Read the result:
        average_result = np.genfromtxt("test_global.txt", dtype='float', delimiter=',', skip_header=1)[:, 1]
        # Some checks
        self.assertEqual(len(project.computations), 1)
        calc = project.computations[0]
        self.assertEqual(calc.result.nsources, 1)
        src = calc.result.sources[0]
        self.assertEqual(calc.result.nreceptors, 4)
        # Compare the average to each receptor:
        for rec in calc.result.receptors:
            rec_result = get_rec_spec(project, [src], [rec])
            np.testing.assert_array_almost_equal(average_result, rec_result[0][0], 6)


if __name__ == '__main__':
    os.chdir(os.path.join(TOOLBOX_DATA_DIR, 'Recepteur_Mobile'))
    unittest.main()
