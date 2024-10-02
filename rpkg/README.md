# REDATAM CONVERTER <img src="man/figures/logo.svg" align="right" height="139" alt="" />

[![Standalone C++ app](https://github.com/pachadotdev/redatam-converter/actions/workflows/build-binaries.yml/badge.svg)](https://github.com/pachadotdev/redatam-converter/actions/workflows/build-binaries.yml)
[![R Package](https://github.com/pachadotdev/redatam-converter/actions/workflows/build-rpkg.yml/badge.svg)](https://github.com/pachadotdev/redatam-converter/actions/workflows/build-rpkg.yml)

The Redatam Converter is an R package for extracting raw information from REDATAM databases.
It is a full R and C++ ground-up rewrite of the original [redatam-converter](https://github.com/discontinuos/redatam-converter/blob/master/README-EN.md).

For the standalone command line application, see the [main directory](https://github.com/pachadotdev/redatam-converter/) of this repository.

To install the R package, run the following command:

```r
remotes::install_github("pachadotdev/redatam-converter/rpkg", subdir = "rpkg")
```

## Credits

This Redatam Converter was created and is supported by Lital Barkai (barkailital@gmail.com).

The tests, installation instructions and R package were created by Mauricio "Pacha" Vargas Sepulveda (m.sepulveda@mail.utoronto.ca)

The original converter was created by [Pablo De Grande](https://github.com/discontinuos). See [here](https://www.scielo.org.mx/scielo.php?script=sci_arttext&pid=S0186-72102016000300811) for more information.
