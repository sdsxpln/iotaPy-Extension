from distutils.core import setup, Extension
setup(name='iotaPy', version='0.1', ext_modules=[Extension('iotaPy', ['iotaPy.c'])])
