# redatam 2.4.0

* The output is a list of tables of `data.table` class.
* Replaces `stringr`/`stringi` with base R functions.
* Handles special characters better.
* Updated vignette.

# redatam 2.3.2

* Using official 'pugixml' fix for GCC 16.

# redatam 2.3.1

* A new GCC 16 warning showed up, so I edited 'pugixml' a bit and sent an
  upstream pull request.
* The vendored 'pugixml' has these changes:
  * Changes in pugixml.cpp:1135 (non-COMPACT #else branch)
  * `xml_attribute_struct`: `header(PUGI_IMPL_GETHEADER_IMPL(this, page, 0))` moved from body to
    initializer list
  * `xml_node_struct`: `header(PUGI_IMPL_GETHEADER_IMPL(this, page, type))` moved from body to
    initializer list
  * `header` is now formally initialized before any use, eliminating GCC 16's false positive
  * The macro only does pointer arithmetic on this and page — no member dereferencing — so using
    this in the initializer list is safe. You can verify with:

# redatam 2.3.0

* New 'pugixml' version that fixes a CRAN R-Devel warning.

# redatam 2.2.0

* Uses 'cpp4r' instead of 'cpp11' for the R-C++ bindings.

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
