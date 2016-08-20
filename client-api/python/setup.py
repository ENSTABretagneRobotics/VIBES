#!/usr/bin/env python
import os
# from distutils.core import setup
from setuptools import setup, Distribution, Extension
import distutils

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

setup(name='vibes',
      version='0.2rc2',
      description='python Api for VIbes (VIsualizer for Intervals and Boxes)',
      long_description=read('README.md'),
      author='EnstaRobotics',
      author_email='ben.ensta@gmail.com',
      url='http://enstabretagnerobotics.github.io/VIBES/',
      packages=['vibes'],
      keywords = ['VIbes', 'Interval Arithmetics', 'Graphics Tools'], # arbitrary keywords
      license = 'LGPLv3',
      classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Topic :: Utilities',
        'Topic :: Scientific/Engineering :: Visualization',
        'Operating System :: OS Independent',
        'License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)',
      ],
)
