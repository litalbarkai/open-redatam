#' @useDynLib redatam, .registration = TRUE
#' @keywords internal
#' @importFrom data.table as.data.table is.data.table setnames `:=` `.SD`
#' @importFrom janitor make_clean_names
#' @importFrom stringi stri_enc_toutf8
#' @importFrom stringr str_trim str_replace_all
#' @importFrom tibble as_tibble
"_PACKAGE"
