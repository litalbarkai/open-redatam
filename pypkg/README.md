# Open Redatam (Python Package) <img src="man/figures/logo.svg" align="right" height="139" alt="" />

[![Python Package](https://github.com/pachadotdev/open-redatam/actions/workflows/build-pypkg.yml/badge.svg)](https://github.com/pachadotdev/open-redatam/actions/workflows/build-pypkg.yml)
[![BuyMeACoffee](https://raw.githubusercontent.com/pachadotdev/buymeacoffee-badges/main/bmc-donate-white.svg)](https://buymeacoffee.com/pacha)

## About

Open Redatam is a software for extracting raw information from REDATAM databases.

For the standalone C++ command line application and desktop app, see the [main directory](https://github.com/pachadotdev/open-redatam/) of this repository.

Please read our article for the full context of this project (Open Access):

> Vargas Sepúlveda, Mauricio and Barkai, Lital. 2025. "The REDATAM format and its challenges for data access and information creation in public policy." *Data & Policy* 7 (January): e18. [https://dx.doi.org/10.1017/dap.2025.4](https://dx.doi.org/10.1017/dap.2025.4). 

Install the Python package using [`uv`](https://docs.astral.sh/uv/):

```bash
git clone https://github.com/pachadotdev/open-redatam.git
cd open-redatam/pypkg

# Sync the environment (creates venv and installs dependencies)
uv sync
```

For developers working on the code:

```bash
# Clean up old build artifacts
rm -rf build dist *.egg-info

# Install package in editable mode
uv pip install -e .

# Test
uv run tests/basic-test.py
```

## Processed data

If you only need the processed data, you can download the [microdata repository](https://github.com/pachadotdev/redatam-microdata/releases). It is available in RDS format for easy loading into R.

Available datasets:

- **Argentina**: 1991, 2001, 2010  
- **Bolivia**: 2001, 2012  
- **Chile**: 2017  
- **Ecuador**: 2010  
- **El Salvador**: 2007  
- **Guatemala**: 2018  
- **Mexico**: 2000  

## Requirements

Python 3.8 or higher.

## Usage

For a given census, such as the [Chilean Census 2017](https://redatam.org/cdr/descargas/censos/poblacion/CP2017CHL.zip), run the following command:

```python
import redatam
redatam.read_redatam("input-dir/dictionary.dicx")
```

Please read the vignette for a more detailed explanation and how this package can be used in conjunction with `dplyr` and other packages.

## Differences with the C++ standalone application

The Python package uses a modified copy of the C++ code to read the REDATAM databases that parses data into dictionary of data frames instead of writing to CSV files.

## Credits

Open Redatam was created and is supported by Lital Barkai (barkailital@gmail.com).

The tests, installation instructions and Python package were created by Mauricio "Pacha" Vargas Sepulveda (m.sepulveda@mail.utoronto.ca)

The original converter was created by [Pablo De Grande](https://github.com/discontinuos). See [here](https://www.scielo.org.mx/scielo.php?script=sci_arttext&pid=S0186-72102016000300811) for more information.

This project uses [pugixml](https://github.com/zeux/pugixml) created by Arseny Kapoulkine to structure a part of the output data.
