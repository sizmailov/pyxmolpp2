PROJECT_TITLE = 'xmolpp2' # Change when C++ doc will be available
PROJECT_SUBTITLE = f'Python API'

MAIN_PROJECT_URL = '../../'

PLUGINS = [
    'm.code',
    'm.py_exec',
    'm.matplotlib_figure',
    'm.components',
    'm.gh',
    'm.htmlsanity',
    'm.math',
    'm.sphinx',

    'm.dox',
    'm.images',
    'm.link',
]

INPUT_MODULES = ['pyxmolpp2', 'pyxmolpp2.pipe']

INPUT_PAGES = ['index.rst',
               'install.rst',
               'changelog.rst',
               'overview.rst',
               'examples/index.rst',
               'examples/rmsd.rst',
               'examples/pdb_files.rst',
               'examples/reorder_atoms.rst',
               'examples/torsion_angle.rst',
               'examples/dat_to_xtc.rst',
               'examples/angles.rst',
               'examples/trajectory_formats.rst',
               'examples/predicate_generators.rst',
               ]

OUTPUT = f'../../../_/site/api/python'

LINKS_NAVBAR1 = [
    ('Install', 'install', [
        ('Changelog', 'changelog'),
    ]),
    ('Docs', 'overview', [
        ('Overview', 'overview'),
        ('Examples', 'examples'),
    ]),
    ('API', 'pyxmolpp2', [
        ('Modules', 'modules'),
        ('Classes', 'classes'),
    ]),
    ('Github', 'https://github.com/sizmailov/pyxmolpp2', [])
]

PYBIND11_COMPATIBILITY = True
M_SPHINX_PARSE_DOCSTRINGS = True
