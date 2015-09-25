import ConfigParser
import io
import json
import os
import tempfile
import unittest

from utils import TympanTC, PROJECT_BASE, TEST_DATA_DIR, TEST_SOLVERS_DIR
from tympan.config_gui import _update_config_with_user_values, _update_user_config
from tympan.models.project import Project
from tympan.models.solver import _set_solver_config
from tympan.models import _solver as cysolver


class TestSolverConfig(TympanTC):

    def test_parameters_serialization(self):
        config = (
            '''
            [tutu]
                titi=15549.15
                toto=hi there!
                tata=0
            ''')
        # Open a basic project that doesn't have solver parameters
        project = self.load_project('', 'empty_site.xml')
        # Set a custom solver configuration to it
        project.current_computation.solver_parameters = config
        # Export to XML and reimport it
        with tempfile.NamedTemporaryFile(suffix='.xml', delete=False) as f:
            project.to_xml(f.name)
            configured_project = Project.from_xml(f.name)
            # Check configuration didn't disappear
            self.assertEqual(configured_project.current_computation.solver_parameters,
                             config)
        f.close()
        os.unlink(f.name)

    def test_set_solver_config(self):
        # Open a basic project with a custom solver configuration
        project = self.load_project('', 'test_solver_params.xml')
        _set_solver_config(project.current_computation)
        solver_config = cysolver.Configuration.get()
        # 10. by default, 20. in the XML file
        self.assertEqual(solver_config.H1parameter, 20.)

    def test_parameters_parsing(self):
        # Open a basic project with a custom solver configuration
        project = self.load_project('', 'test_solver_params.xml')
        _set_solver_config(project.current_computation)
        solver_config = cysolver.Configuration.get()
        # bool
        self.assertEqual(solver_config.UseReflection, True)
        self.assertEqual(solver_config.PropaConditions, False)
        # float
        self.assertEqual(solver_config.H1parameter, 20.)
        # double
        self.assertEqual(solver_config.AnalyticDMax, 3000.)
        # int
        self.assertEqual(solver_config.Anime3DForceC, 1)


class TestSolverConfigGUI(unittest.TestCase):
    def setUp(self):
        datamodel_filepath = os.path.join(PROJECT_BASE, 'resources', 'solver_config_datamodel.json')
        with open(datamodel_filepath) as stream:
            self.config_model = json.load(stream)
        user_config = io.StringIO(
            u'[DEFAULTSOLVER]\nNbThreads=True\nUseRealGround=False\n[ANALYTICRAYTRACER]\n'
            'CurveRaySampler=2.3\nInitialAngleTheta=1\nblop=whatever')
        self.conf_parser = ConfigParser.ConfigParser()
        self.conf_parser.readfp(user_config)

    def test_update_config_with_user_values(self):
        _update_config_with_user_values(self.config_model, self.conf_parser)
        self.assertEqual(self.config_model['DEFAULTSOLVER']['NbThreads']['default'], 4)
        self.assertEqual(self.config_model['DEFAULTSOLVER']['NbThreads']['value'], 4)
        self.assertEqual(self.config_model['DEFAULTSOLVER']['UseRealGround']['default'], True)
        self.assertEqual(self.config_model['DEFAULTSOLVER']['UseRealGround']['value'], False)
        self.assertEqual(self.config_model['ANALYTICRAYTRACER']['CurveRaySampler']['default'], 1)
        self.assertEqual(self.config_model['ANALYTICRAYTRACER']['CurveRaySampler']['value'], 1)
        self.assertEqual(self.config_model['ANALYTICRAYTRACER']['InitialAngleTheta']['default'], 0.0)
        self.assertEqual(self.config_model['ANALYTICRAYTRACER']['InitialAngleTheta']['value'], 1.0)
        self.assertNotIn('blop', self.config_model['ANALYTICRAYTRACER'])

    def test_update_user_config(self):
        _update_config_with_user_values(self.config_model, self.conf_parser)
        self.config_model['DEFAULTSOLVER']['NbThreads']['value'] = 1
        self.config_model['ANALYTICRAYTRACER']['CurveRaySampler']['value'] = 3
        self.config_model['ANALYTICRAYTRACER']['InitialAngleTheta']['value'] = 5.
        _update_user_config(self.config_model, self.conf_parser)
        self.assertEqual(self.conf_parser.getint('DEFAULTSOLVER', 'NbThreads'), 1)
        self.assertEqual(self.conf_parser.getboolean('DEFAULTSOLVER', 'UseRealGround'), False)
        self.assertEqual(self.conf_parser.getint('ANALYTICRAYTRACER', 'CurveRaySampler'), 3)
        self.assertEqual(self.conf_parser.getfloat('ANALYTICRAYTRACER', 'InitialAngleTheta'), 5.0)
        self.assertEqual(self.conf_parser.get('ANALYTICRAYTRACER', 'blop'), 'whatever')


class TestSolverSelection(TympanTC):

    def test_select_solver(self):
        project = Project.from_xml(os.path.join(TEST_DATA_DIR, 'empty_site.xml'))
        self.assertEqual(project.current_computation.solver_id,
                         "{a98b320c-44c4-47a9-b689-1dd352daa8b2}")
        project.set_solver('ANIME3DSolver', solverdir=TEST_SOLVERS_DIR)
        self.assertEqual(project.current_computation.solver_id,
                         "{2f0b51a7-37bd-414e-8908-baea85acef2c}")


if __name__ == '__main__':
    import unittest
    unittest.main()
