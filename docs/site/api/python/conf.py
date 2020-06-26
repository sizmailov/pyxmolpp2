import pyxmolpp2
import glob

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
               'examples.rst',
               'overview.rst'
               ]

OUTPUT = f'../../../_/site/api/python'

LINKS_NAVBAR1 = [
    ('Install', 'install', [
        ('Changelog', 'changelog'),
    ]),
    ('Overview', 'overview', []),
    ('Examples', 'examples', []),
    ('API', 'pyxmolpp2', [
        ('Modules', 'modules'),
        ('Classes', 'classes'),
    ]),
    ('Github', 'https://github.com/sizmailov/pyxmolpp2', [])
]

PYBIND11_COMPATIBILITY = True
