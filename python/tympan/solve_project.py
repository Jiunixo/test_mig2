import sys
import os
import io
import logging
import ConfigParser
from StringIO import StringIO

# open file in unbuffered mode so it get written asap, in case of later crash
# due to underlying C code
stream = open('tympan.log', 'a', 0)
logging.basicConfig(stream=stream, level=logging.DEBUG,
                    format='%(levelname)s:%(asctime)s - %(name)s - %(message)s')


try:
    import tympan.models.business as tybusiness
except ImportError:
    err = "solve_project.py module couldn't find tympan.models.business cython library."
    logging.critical("%s Check PYTHONPATH and path to Tympan libraries.", err)
    raise ImportError(err)

try:
    import tympan.business2solver as bus2solv
except ImportError:
    err = "solve_project.py module couldn't find tympan.business2solver cython library."
    logging.critical("%s Check PYTHONPATH and path to Tympan libraries.", err)
    raise ImportError(err)

from tympan import SOLVER_CONFIG_ATTRIBUTES
from tympan.altimetry.builder import Builder
from tympan.altimetry import process_altimetry
from tympan.models.solver import Configuration

CONVERTERS = {
    'bool': bool,
    'int': int,
    'float': float,
    'double': float,
}

CONFIG_MAP = dict((optname, CONVERTERS[opttype]) for opttype, optname in SOLVER_CONFIG_ATTRIBUTES)


def solve(input_project, output_project, output_mesh, solverdir,
          multithreading_on=True,
          interactive=False):
    """ Solve an acoustic problem with Code_TYMPAN from

        Keywords arguments:
        input_project -- XML file containing the serialized project with the
            "calcul" to solve
        output_project -- XML file where to put the project updated with the
            results of the computation
        output_mesh -- a file in which to put the altimetry mesh once computed (ply format)
        solvedir -- directory containing the solver plugin
        -------
        optional (debug):
        multithreading_on -- set it to False to solve the acoustic problem with only
            one thread
        interactive -- if True, pdb debugger will be invoked before running solving
            the acoustic problem, so that the program can be executed in interactive
            mode.

        The execution is logged into 'tympan.log', created in the directory of
        the input XML project (the one opened from the Code_TYMPAN GUI)
    """
    if interactive:
        import pdb
        pdb.set_trace()
    ret = False
    tybusiness.init_tympan_registry()
    # Load an existing project and retrieve its calcul to solve it
    try:
        project = tybusiness.Project.from_xml(input_project)
    except RuntimeError:
        logging.exception("Couldn't load the acoustic project from %s file", input_project)
        raise
    # Business model
    site = project.site
    comp = project.current_computation
    # Solver model
    solver_problem = comp.acoustic_problem
    solver_result = comp.acoustic_result
    parser = ConfigParser.RawConfigParser()
    # keep param names case
    parser.optionxform = str
    # Setup solver configuration
    parser.readfp(StringIO(comp.solver_parameters))
    solver_config = Configuration.get()
    errors = []
    for section in parser.sections():
        for optname, value in parser.items(section):#solver_config.items(section):
            try:
                value = CONFIG_MAP[optname](value)
            except ValueError:
                errors.append('bad option value for %s: %r' % (optname, value))
                continue
            getattr(solver_config, optname, value)
    if errors:
        raise ConfigParser.Error(os.linesep.join(errors))
    if not multithreading_on:
        solver_config.NbThreads = 1
    # Recompute altimetry
    # Rebuild topography with altimetry data model
    alti_site = process_altimetry.export_site_topo(site)
    # Compute altimetry and retrieve the resulting mesh
    builder =  Builder(alti_site)
    builder.complete_processing()
    builder.export_to_ply(output_mesh)
    vertices, faces, materials, faces_materials = builder.build_mesh_data()
    # Update site and the project before building the solver model
    site.update_altimetry(vertices, faces, materials, faces_materials)
    project.update()
    # Build an acoustic problem from the site of the computation
    bus2solv_conv = bus2solv.Business2SolverConverter(comp, project.site)
    bus2solv_conv.build_solver_problem()
    logging.info("Solver model built.\nNumber of sources: %d\nNumber of receptors: %d",
                 bus2solv_conv.nsources, bus2solv_conv.nreceptors)
    if (bus2solv_conv.nsources == 0 or bus2solv_conv.nreceptors == 0):
        err = "You must have at least one source and one receptor to run a simulation."
        raise RuntimeError(err)
    # Load solver plugin
    solver = bus2solv.load_computation_solver(solverdir, comp)
    # Solve the problem and fill the acoustic result
    logging.debug("Calling C++ go method")
    ret = comp.go(solver)
    if ret is False:
        err = "Computation failed (C++ go method returned false)"
        logging.error(err)
        raise RuntimeError(err)
    # Export solver results to the business model
    bus2solv_conv.postprocessing()
    # Reserialize project
    try:
        project.to_xml(output_project)
    except ValueError:
        logging.exception("Couldn't export the acoustic results to %s file", output_project)
        raise
