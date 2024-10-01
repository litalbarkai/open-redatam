#' @title Read REDATAM dictionary
#' @description Read a DIC or DICX dictionary file directly into R.
#' @param dictionary A character string with the path to the dictionary file. It
#'  allows relative paths.
#' @param labels_as_factors Logical. If FALSE, the labels are kept as tags and
#'  additional tables with their descriptions are provided If TRUE, the labels
#'  are converted to factors (e.g., labels such as "01" become "Montevideo").
#'  The default is FALSE as it is more flexible and converting to factors can
#'  without filtering the tables can be slow.
#' @return A list of tibbles, each one representing a table with the census
#'  entities (or levels) and their attributes (or variables).
#' @examples
#' \dontrun{
#' # Read a dictionary file (Uruguay 2011, downloaded from redatam.org)
#' read_redatam("CP2011URY/BaseRPub/CPV2011_uruguay_publica.dicX")
#' }
#' @export
read_redatam <- function(dictionary, labels_as_factors = FALSE) {
  dictionary <- normalizePath(dictionary)

  if (grepl("\\.dic$|\\.DIC$|\\.dicx$|\\.DICX$||\\.dicX", dictionary)) {
    res <- export_redatam_to_list_(dictionary)
  } else {
    stop("The file extension must be either .dic or .dicx")
  }

  message("Fixing encoding...")

  # 1. convert lists to data.tables
  res <- list_to_datatable_(res)
  res <- res[sapply(res, nrow) > 0]

  # 2. fix encoding (e.g., ""C\xf3digo Region" -> "Código Region")
  res <- fix_encoding_(res)

  message("Tidying names...")

  # 3. convert REGION, Region, Regi\u00f3n, etc. to region and so on
  res <- tidy_names_(res)

  message("Tidying variables...")

  # 4. remove leading and trailing whitespaces and multiple whitespaces
  res <- trim_and_clean_(res)

  # 6. harmonize column types
  res <- harmonize_types_(res)

  # 7. merge with their descriptions
  res <- merge_descriptions_(res, labels_as_factors)
  if (isTRUE(labels_as_factors)) {
    res <- remove_extra_labels_(res)
  }

  # 8. show as tibble
  res <- datatable_to_tibble_(res)

  # 9. put entities first
  res <- res[sort(names(res))]

  return(res)
}
