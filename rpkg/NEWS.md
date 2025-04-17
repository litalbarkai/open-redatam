# redatam 2.1.2
* Fixes a bug with .DIC/.DICX files that are not accepted when these are
  not in lower case (e.g., file.dic or file.dicx).

# redatam 2.1.0

* Streamlined the C++ code. Minimal performance improvements, but it is now
  easier to read.

# redatam 2.0.4

* Refactored C++ to R list casting to avoid growing a list in a loop. It now
  creates a list with a lenght equal to the number of entities and variables
  with descriptions and then fills it with the data. This is 3 to 5 times
  faster.
* Fixes GCC-SAN errors with a fix proposed by Ivan Krylov. Thanks!

# redatam 2.0.0

* First CRAN release.

# redatam 0.2.1

* CRAN requested changes:
  * Added Arseny Kapoulkine to 'ctb'
  * Added the Republic of Ecuador  to 'dtc'
  * Removed \dontrun{} from examples in the main function

# redatam 0.2

* Refactored to use C++11 instead of C++17.

# redatam 0.1

* First working version.
* Initial CRAN submission.
* Tested with the following census:
  * Chile 2017 (DIC and DICX)
  * Uruguay 2011 (DICX)
  * Galapagos, Ecuador 2015 (original in DIC reduced to macro levels with
    Redatam 7 and DICX created from the DIC file)
