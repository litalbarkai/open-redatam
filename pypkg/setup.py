from setuptools import setup, Extension, find_packages
import pybind11

# Extension module
ext_modules = [
    Extension(
        'redatam._redatam',  # Private compiled module
        sources=[
            'vendor/pugixml.cpp',
            'redatamlib/BitArrayReader.cpp',
            'redatamlib/ByteArrayReader.cpp',
            'redatamlib/Entity.cpp',
            'redatamlib/FuzzyEntityParser.cpp',
            'redatamlib/FuzzyVariableParser.cpp',
            'redatamlib/ParentIDCalculator.cpp',
            'redatamlib/PyDictExporter.cpp',
            'redatamlib/RedatamDatabase.cpp',
            'redatamlib/Variable.cpp',
            'redatamlib/XMLParser.cpp',
            'redatamlib/utils.cpp',
            'main.cpp'
        ],
        include_dirs=[pybind11.get_include(), 'vendor', 'redatamlib'],
        extra_compile_args=['-std=c++11', '-O3'],
        language='c++'
    ),
]

setup(
    name='redatam',
    version='0.1',
    packages=find_packages(include=['redatam']),
    ext_modules=ext_modules,
    setup_requires=['pybind11>=2.5.0'],
    install_requires=[
        'pandas',
        'numpy',
        'chardet'
    ],
    zip_safe=False
)
