# REDATAM CONVERTER

## About

Program for extracting raw information from REDATAM databases.
This program is a CLI adaptation of the original [redatam-converter](https://github.com/discontinuos/redatam-converter).

Redatam Converter is an open source project that lets you read Redatam databases in order to export their microdata. It was created to recover information of REDATAM databases for statistical analysis using standard tools such as SPSS, STATA, R, etc.

## Compatibility

The converter was tested with REDATAM databases of different versions.
The current version exports to comma separated files (CSV).

## Prerequisites

This program uses **Xerces-C++ XML Parser** and requires C++17 or higher to compile.
See [here](https://xerces.apache.org/xerces-c/) for download and installation guide.

We provide a script for Ubuntu users to install Xerces-C++ XML Parser. Run the following command:

```bash
bash dev/01-install-xerces.sh
```

## Installation

### From binaries

On Ubuntu, run the following commands:

```bash
# needs "sudo apt install gdebi-core" if you don't have gdebi
wget https://github.com/pachadotdev/redatam-converter/releases/download/v0.0.1/redatam_0.0.1_amd64.deb
sudo dpkg -i redatam_0.0.1_amd64.deb
```

This will install `redatam` and `redatamgui` in `/usr/local/bin/` with the necessary dependencies and a desktop entry.

On Mac, run the following command:

```bash
# needs "brew", install it from https://brew.sh/ if you don't have it
sudo bash -c "$(wget -qO- https://raw.githubusercontent.com/pachadotdev/redatam-converter/main/install/mac.sh)"
```

This will install `redatam` and `redatamgui` in `/usr/local/bin/` with the necessary dependencies.

### From source

On Linux, run the following commands:

```bash
git clone https://github.com/pachadotdev/redatam-converter.git
sudo apt-get update
sudo apt-get install -y qtbase5-dev qtbase5-dev-tools qt5-qmake
bash dev/01-install-xerces.sh
make
```

Then run `./redatam` or `./redatamgui`.

On Mac, run the following commands:

```bash
git clone https://github.com/pachadotdev/redatam-converter.git
brew install wxwidgets
echo "$(brew --prefix wxwidgets)/bin" >> $GITHUB_PATH
echo "PKG_CONFIG_PATH=$(brew --prefix wxwidgets)/lib/pkgconfig" >> $GITHUB_ENV
bash dev/01-install-xerces.sh
make
```

Then run `./redatam` or `./redatamgui`.

## Usage

To use the program, run the following command:

```bash
redatam input.dic(x) output-csv/
```

For the [Chilean Census 2017](https://redatam.org/cdr/descargas/censos/poblacion/CP2017CHL.zip), this would be `redatam downloads/CP2017CHL/BaseOrg16/CPV2017-16.dicx Chile_2017/`.

## Testing

Tested on Ubuntu 22.04.

Ticked = Passed; Blank = Failed

### DIC format

- [ ] Argentina 2010 (`CP2010ARG/BASE_AMP_DPTO/CPV2010Ampliado.dic`)
- [ ] Bolivia 2001 (`CP2001BOL/Cp2001BOL/BaseOriginal/CPV2001.dic`)
- [ ] Bolivia 2001 CELADE (`CP2001BOL/Cp2001BOL/Celade/CPV2001BOL_Celade.dic`)
- [x] Bolivia 2012 (`CP2012BOL/BaseMunicipio_V3/CPV2012Municipio.dic`)
- [x] Chile 2017 (`CP2017CHL/BaseOrg16/CPV2017-16.dic`)
- [x] Dominican Republic 2002 (`CP2002DOM/Cp2002DOM/BaseOriginal/CPV2002DOM.dic`)
- [x] Ecuador 2010 (`downloads/CP2010ECU/Base/CE11.dic`)
- [x] El Salvador 2007 (`CP2007SLV/CP2007SLV/BaseTotal/CPV2007ES.dic`)
- [ ] Guatemala 2018 (`CP2018GTM/BasePub/CPV2018GT_BasePublica.dic`)
- [x] Mexico 2000 (`downloads/CP2000MEX/Cp2000MEX/BaseOriginal/cpmx2000.dic`)
- [ ] Mexico 2010 (`CP2010MEX/BasePubM/MC10.dic`)
- [x] Myanmar 2014 (`CP2014MMR/Union.dic`)
- [ ] Peru 2010 (`CP2010PER/CP2007PER/BasePub/CPV2007PER_PUB.dic`)
- [ ] Peru 2017 (`CP2017PER/BaseD/BaseR/CPVPER2017D.dic`)
- [ ] Uruguay 2011 (`CP2000MEX/Cp2000MEX/BaseOriginal/cpmx2000.dic`) *corrupted file*

### DICX format

- [x] Argentina 2010 (`CP2010ARG/BASE_AMP_DPTO/CPV2010Ampliado.dicx`)
- [ ] Bolivia 2001 (`CP2001BOL/Cp2001BOL/BaseOriginal/CPV2001.dicx`)
- [ ] Bolivia 2001 CELADE (`CP2001BOL/Cp2001BOL/Celade/CPV2001BOL_Celade.dicx`)
- [x] Bolivia 2012 (`CP2012BOL/BaseMunicipio_V3/CPV2012Comunidad.dicx`)
- [x] Chile 2017 (`CP2017CHL/BaseOrg16/CPV2017-16.dicx`)
- [x] Dominican Republic 2002 (`CP2002DOM/Cp2002DOM/BaseOriginal/CPV2002DOM.dicx`)
- [x] Ecuador 2010 (`CP2010ECU/Base/cpv2010ecu.dicx`)
- [x] El Salvador 2007 (`CP2007SLV/CP2007SLV/BaseTotal/CPV2007ES.dicx`)
- [x] Mexico 2000 (`CP2000MEX/Cp2000MEX/BaseOriginal/cpmx2000.dicx`)
- [x] Mexico 2010 (`CP2010MEX/BasePubM/MC10.dicx`)
- [x] Mexico 2015 (`ECI2015MEX/BaseR/ECI2015MX.dicX`)
- [x] Peru 2010 (`CP2010PER/CP2007PER/BasePub/CPV2007PER_PUB.dicx`)
- [x] Uruguay 2011 (`CP2011URY/CP2011URY/BaseRPub/CPV2011_uruguay_publica.dicX`)

## Credits

This Redatam Converter was created and is supported by Lital Barkai (barkailital@gmail.com).

The original converter was created by [Pablo De Grande](https://github.com/discontinuos). See [here](https://www.scielo.org.mx/scielo.php?script=sci_arttext&pid=S0186-72102016000300811) for more information.

The tests and installation instructions were created by Mauricio "Pacha" Vargas Sepulveda (m.sepulveda@mail.utoronto.ca)
