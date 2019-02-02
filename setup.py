import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion

from write_version_info import get_version_info

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
        if cmake_version < LooseVersion('3.5.0'):
            raise RuntimeError("CMake >= 3.5.0 is required")

        for ext in self.extensions:
            self.build_extension(ext)


        subprocess.call([sys.executable,
                         os.path.join(os.path.dirname(__file__), "external", "pybind11-stubgen",
                                      "pybind11_stubgen.py"),
                         "--output-dir", os.path.dirname(os.path.abspath(__file__)),
                         "--no-setup-py",
                         "pyxmolpp2"
                         ])

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        build_type = os.environ.get("BUILD_TYPE", "Release")
        build_args = ['--config', build_type]

        # Pile all .so in one place and use $ORIGIN as RPATH
        cmake_args += ["-DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE"]
        cmake_args += ["-DCMAKE_INSTALL_RPATH={}".format("$ORIGIN")]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(build_type.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + build_type]
            build_args += ['--', '-j4']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake',
                               '--build', '.',
                               '--target', ext.name
                               ] + build_args,
                              cwd=self.build_temp)

def find_stubs(package):
    stubs = []
    for root, dirs, files in os.walk(package):
        for file in files:
            path = os.path.join(root, file).replace(package + os.sep, '', 1)
            stubs.append(path)
    return {package: stubs}


def build_ext_before(command):
    class Build_ext_first(command):
        def run(self):
            self.run_command("build_ext")
            return command.run(self)
    return Build_ext_first

setup(
    name='pyxmolpp2',
    version=get_version_info()[3],
    author='Sergei Izmailov',
    author_email='sergei.a.izmailov@gmail.com',
    description='Utils for processing MD',
    long_description=open("README.rst").read(),
    ext_modules=[CMakeExtension('pyxmolpp2')],
    packages=find_packages(),
    cmdclass=dict(build_ext=CMakeBuild),
    url="https://github.com/sizmailov/pyxmolpp2",
    zip_safe=False,
    package_data={
        'pyxmolpp2':
            find_stubs('pyxmolpp2-stubs'),
    }
)
