#' @title Import 'REDATAM' Files
#' @description Import 'REDATAM' formats into R via the 'Open REDATAM' C++ library.
#'  The full context of this project and details about the implementation are
#'  available in <doi:10.1017/dap.2025.4> (Open Access).
#' @useDynLib redatam, .registration = TRUE
#' @keywords internal
#' @importFrom data.table is.data.table setnames setDT `:=` `.SD`
#' @importFrom janitor make_clean_names
"_PACKAGE"
