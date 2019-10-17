#!/usr/bin/env python3
"""Create setup script for installation of embed_fast5"""
########################################################################
# File: setup.py
#  executable: setup.py
#
# Author: Andrew Bailey (https://www.benjack.io/2018/02/02/python-cpp-revisited.html)
# History: 3/21/19 Created
########################################################################

import os
import re
import sys
import platform
import subprocess
from shutil import copyfile, copymode
from timeit import default_timer as timer

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install

from distutils.version import LooseVersion

_test_file = 'test_event_detect_cpp'
_main_executable = 'nanopore-cli'


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


def copy_test_file(src_file):
    """
    Copy ``src_file`` to `tests/bin` directory, ensuring parent directory
    exists. Messages like `creating directory /path/to/package` and
    `copying directory /src/path/to/package -> path/to/package` are
    displayed on standard output. Adapted from scikit-build.
    """
    # Create directory if needed
    dest_dir = os.path.join(os.path.dirname(
        os.path.abspath(__file__)), 'tests', 'bin')
    if dest_dir != "" and not os.path.exists(dest_dir):
        print("creating directory {}".format(dest_dir))
        os.makedirs(dest_dir)

    # Copy file
    dest_file = os.path.join(dest_dir, os.path.basename(src_file))
    print("copying {} -> {}".format(src_file, dest_file))
    copyfile(src_file, dest_file)
    copymode(src_file, dest_file)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2 ** 32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j8']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)
        # Copy *_test file to tests directory
        cpp_tests = os.path.join(self.build_temp, _test_file)
        cpp_main = os.path.join(self.build_temp, _main_executable)
        copy_test_file(cpp_tests)
        copy_test_file(cpp_main)
        print()  # Add empty line for nicer output


class PostInstallCommand(install):
    """Post-installation for installation mode."""
    def run(self):
        install.run(self)
        build_temp = self.build_lib.replace("lib", "temp")
        source = os.path.join(os.path.dirname(os.path.abspath(__file__)), build_temp, _main_executable)
        target = os.path.join(self.install_scripts, _main_executable)
        if os.path.isfile(target):
            os.remove(target)

        self.copy_file(source, target)


def main():
    """Main docstring"""
    start = timer()
    setup(
        name="nanopore-toolkit",
        version='0.0.1',
        description='Nanopore toolkit for dealing with nanopore data',
        url="https://github.com/UCSC-nanopore-cgl/nanopore-toolkit",
        author='Andrew Bailey',
        license='MIT',
        packages=find_packages('src'),
        package_dir={'': 'src'},
        ext_modules=[CMakeExtension('ntk.bindings')],
        cmdclass=dict(build_ext=CMakeBuild, install=PostInstallCommand),
        # scripts=[],
        author_email='andbaile@ucsc.com',
        # install_requires=['py3helpers[seq_tools]>=0.2.9',
        #                   'pandas>=0.24.2',
        #                   'h5py>=2.9.0'],
        zip_safe=False,
        test_suite='tests'
    )

    stop = timer()
    print("Running Time = {} seconds".format(stop - start), file=sys.stderr)


if __name__ == "__main__":
    main()
    raise SystemExit
