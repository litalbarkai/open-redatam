# REDATAM CONVERTER
Program for extracting raw information from REDATAM databases.
This program is a CLI adaptation of the original [redatam-converter](https://github.com/discontinuos/redatam-converter).

Redatam Converter is an open source project that lets you read Redatam databases in order to export their microdata. It was created to recover information of REDATAM databases for statistical analysis using standard tools such as SPSS, STATA, R, etc.

# COMPATIBILITY
The converter was tested with REDATAM databases of different versions.
The current version exports to comma separated files (CSV).

# PREREQUISITES
This program uses **Xerces-C++ XML Parser**.
See [here](https://xerces.apache.org/xerces-c/) for download and installation guide.

**MAKEFILE**: make sure to change XERCES_PATH to where xerces is installed.


# CREDITS
This Redatam Converter was created and is supported by Lital Barkai (barkailital@gmail.com).

The original converter was created by [Pablo De Grande](https://github.com/discontinuos). See [here](https://www.scielo.org.mx/scielo.php?script=sci_arttext&pid=S0186-72102016000300811) for more information.
