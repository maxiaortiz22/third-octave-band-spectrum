from distutils.core import setup, Extension
import numpy as np

#Genero el wrapper:
name = "spectrum"      # name of the module
version = "1.0"  # the module's version number

setup(name=name, version=version,
      ext_modules=[Extension(name='_spectrum', 
      # SWIG requires an underscore as a prefix for the module name
             sources=["spectrum_wrapper.i", "code/spectrum.c",
                      "code/kiss_fft.c", "code/kiss_fftr.c"],# "usoundlib_params.h"],
             #include_dirs=[numpy.get_include(), "code"])
             include_dirs=[np.get_include(), "code"])
    ])

"""
# compile module
python setup.py build_ext 
# install in the current directory
python setup.py install --install-platlib=.
and test with:

python -c 'import hw'     # test module
"""