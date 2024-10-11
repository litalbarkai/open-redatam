#!/bin/bash

# the original link https://redatam.org/es/microdatos
# is broken since Sep 1, 2024

# We use the Wayback Machine to get the latest snapshot
# https://web.archive.org/web/20240420134748/https://redatam.org/es/microdatos
# the download links are still valid as of Sep 10, 2024

# For Argentina we also checked datar.info
# Links valid on Oct 10, 2024
# Needs browser download to use yandex/mega

# For Chile we also checked ine.gob.cl
# Links valid on Oct 10, 2024
# Needs browser download because of protected links

# For Ecuador we also checked ecuadorencifras.gob.ec
# Links valid on Oct 10, 2024

# Create the downloads directory if it doesn't exist
mkdir -p downloads

# Argentina
# 1991: https://disk.yandex.com/d/mAK9YN6hrzkGA
# 2001: https://mega.nz/file/SMcwiLLA#N7QiIQXOgDwXTwX0r8Yf8ZsAnFh2SzpyuujpREuufXU
if [ ! -f downloads/CP2010ARG.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2010ARG.zip -O downloads/CP2010ARG.zip
fi

# Brasil
if [ ! -f downloads/CP1991BRA.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/Cp1991BRA.zip -O downloads/CP1991BRA.zip
fi

if [ ! -f downloads/CP2000BRA.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/Cp2000BRA.zip -O downloads/CP2000BRA.zip
fi

if [ ! -f downloads/CP2010BRA.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2010BRA.zip -O downloads/CP2010BRA.zip
fi

# Bolivia
if [ ! -f downloads/CP2001BOL.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/Cp2001BOL.zip -O downloads/CP2001BOL.zip
fi

if [ ! -f downloads/CP2012BOL.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2012BOL.zip -O downloads/CP2012BOL.zip
fi

# Chile

# https://www.ine.gob.cl/docs/default-source/censo-de-poblacion-y-vivienda/bbdd/censo-2017/csv/csv-identificaci%C3%B3n-geogr%C3%A1fica-censo-2017.rar?sfvrsn=1ae6f56c_2&download=true
# https://www.ine.gob.cl/docs/default-source/censo-de-poblacion-y-vivienda/bbdd/censo-2017/csv/csv-viviendas-censo-2017.rar?sfvrsn=d741a14a_2&download=true
# https://www.ine.gob.cl/docs/default-source/censo-de-poblacion-y-vivienda/bbdd/censo-2017/csv/csv-hogares-censo-2017.rar?sfvrsn=4f1ab5d3_2&download=true
# https://www.ine.gob.cl/docs/default-source/censo-de-poblacion-y-vivienda/bbdd/censo-2017/csv/csv-personas-censo-2017.rar?sfvrsn=60c6e91c_2&download=true
# https://www.ine.gob.cl/docs/default-source/censo-de-poblacion-y-vivienda/bbdd/censo-2017/csv/csv-manzana---entidad-censo-2017.rar?sfvrsn=1ce846c8_2&download=true

if [ ! -f downloads/CP2017CHL.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2017CHL.zip -O downloads/CP2017CHL.zip
fi

# Ecuador
if [ ! -f downloads/CP2010ECU.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2010ECU.zip -O downloads/CP2010ECU.zip
fi

if [ ! -f downloads/CP2001ECU-SPSS.zip ]; then
  wget https://www.ecuadorencifras.gob.ec/documentos/datos/Poblacion_y_Demografia/Censo_poblacion_y_vivienda_/bdd_nivel_manzana_censal/Spss/CPV2010M_SPSS_Nacional.zip -O downloads/CP2001ECU-SPSS.zip
fi

if [ ! -f downloads/CP2015ECUGalapagos.zip ]; then
  wget https://www.ecuadorencifras.gob.ec/documentos/datos/Poblacion_y_Demografia/Censo_poblacion_y_vivienda_/Galapagos_2015/bdd_CPV_Galapagos_AL15_AT_REDATAM.zip -O downloads/CP2015ECUGalapagos.zip
fi

if [ ! - f downloads/CP2015ECUGalapagos-SPSS.zip ]; then
  wget https://www.ecuadorencifras.gob.ec/documentos/datos/Poblacion_y_Demografia/Censo_poblacion_y_vivienda_/Galapagos_2015/bdd_CPV_Galapagos_2015_AT_SPSS.zip -O downloads/CP2015ECUGalapagos-SPSS.zip
fi

# El Salvador
if [ ! -f downloads/CP2007SLV.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2007SLV.zip -O downloads/CP2007SLV.zip
fi

# Guatemala
if [ ! -f downloads/CP2018GTM.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2018GTM.zip -O downloads/CP2018GTM.zip
fi

# Mexico
if [ ! -f downloads/CP2000MEX.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2000MEX.zip -O downloads/CP2000MEX.zip
fi

if [ ! -f downloads/CP2010MEX.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2010MEX.zip -O downloads/CP2010MEX.zip
fi

if [ ! -f downloads/ECI2015MEX.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/ECI2015MEX.zip -O downloads/ECI2015MEX.zip
fi

if [ ! -f downloads/CP2020MEX.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2020MEX.zip -O downloads/CP2020MEX.zip
fi

# Myanmar
if [ ! -f downloads/CP2014MMR.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2014MMR.zip -O downloads/CP2014MMR.zip
fi

# Peru
if [ ! -f downloads/CP2007PER.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2007PER.zip -O downloads/CP2007PER.zip
fi

if [ ! -f downloads/CP2017PER.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2017PER.zip -O downloads/CP2017PER.zip
fi

# Republica Dominicana
if [ ! -f downloads/CP2002DOM.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2002DOM.zip -O downloads/CP2002DOM.zip
fi

# Uruguay
if [ ! -f downloads/CP2011URY.zip ]; then
  wget https://redatam.org/cdr/descargas/censos/poblacion/CP2011URY.zip -O downloads/CP2011URY.zip
fi
