#' @title Read REDATAM dictionary
#' @description Read a DIC or DICX dictionary file directly into R.
#' @param dictionary A character string with the path to the dictionary file. It
#'  allows relative paths.
#' @return A list of tibbles, each one representing a table with the census
#'  entities (or levels) and their attributes (or variables).
#' @examples
#' \dontrun{
#' # Read a dictionary file (Uruguay 2011, downloaded from redatam.org)
#' read_redatam("CP2011URY/BaseRPub/CPV2011_uruguay_publica.dicX")
#' }
#' @export
read_redatam <- function(dictionary) {
  dictionary <- normalizePath(dictionary)

  if (grepl("\\.dic$|\\.DIC$|\\.dicx$|\\.DICX$||\\.dicX", dictionary)) {
    res <- export_redatam_to_list_(dictionary)
  } else {
    stop("The file extension must be either .dic or .dicx")
  }

  # 1. fix encoding (e.g., ""C\xf3digo Region" -> "Código Region")
  res <- fix_encoding_(res)

  # 2. if an element is an empty list, remove it
  res <- replace_empty_with_na_(res)

  # 3. convert REGION, Region, Regi\u00f3n, etc. to region and so on
  res <- tidy_names_(res)

  # 4. remove leading and trailing whitespaces and multiple whitespaces
  res <- trim_and_clean_(res)

  # 5. present entities as tables
  res <- list_to_tibble_(res)

  # return excluding tables with 0 rows
  return(res[sapply(res, nrow) > 0])
}
