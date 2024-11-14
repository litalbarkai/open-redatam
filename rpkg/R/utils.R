list_to_datatable_ <- function(x) {
  lapply(x, function(y) {
    if (is.list(y)) as.data.table(y) else y
  })
}

# Encoding fix ----

fix_encoding_recursive_ <- function(x) {
  if (is.list(x)) {
    return(lapply(x, fix_encoding_recursive_))
  } else if (is.character(x)) {
    replacements <- c(
      "\u00c3\u00a1" = "\u00e1", # a with acute accent
      "\u00c3\u00a9" = "\u00e9", # e with acute accent
      "\u00c3\u00b3" = "\u00f3", # o with acute accent
      "\u00c3\u00ba" = "\u00fa", # u with acute accent
      "\u00c3\u00b1" = "\u00f1", # n with tilde
      "\u00b1" = "\u00f1", # n with tilde
      "\u00c3" = "\u00ed" # i with acute accent
    )
    return(str_replace_all(stri_enc_toutf8(x), replacements))
  }
  x
}

fix_encoding_in_data_table <- function(y) {
  char_cols <- names(y)[vapply(y, is.character, logical(1))]
  y[, (char_cols) := lapply(.SD, fix_encoding_recursive_), .SDcols = char_cols]
  y
}

# Extract the unique values of the "chr" vectors in the lists
extract_strings <- function(y) {
  if (is.data.table(y)) {
    return(unlist(y[, lapply(.SD, function(col) if (is.character(col)) col else NULL)]))
  } else {
    return(unlist(y[vapply(y, is.character, logical(1))]))
  }
}

fix_encoding_ <- function(x) {
  strings <- unique(unlist(lapply(x, extract_strings)))

  lapply(x, function(y) {
    if (is.data.table(y)) {
      return(fix_encoding_in_data_table(y))
    } else {
      return(fix_encoding_recursive_(y))
    }
  })
}

# Tidy names ----

tidy_names_ <- function(x) {
  if (is.list(x)) {
    element_names <- names(x)

    # check for NULL or empty names and replace them with temporary placeholders
    if (is.null(element_names)) {
      element_names <- rep("", length(x))
    }

    # apply janitor to the non-empty names
    cleaned_names <- ifelse(element_names == "", element_names, make_clean_names(element_names))
    names(x) <- cleaned_names

    # apply tidy_names_ recursively
    return(
      lapply(x, function(y) {
        if (is.data.table(y)) {
          setnames(y, make_clean_names(names(y)))
          return(y)
        } else if (is.list(y)) {
          return(tidy_names_(y))
        } else {
          return(y)
        }
      })
    )
  } else {
    return(x)
  }
}

# Fix multiple spaces ----

# trim leading/trailing spaces and replace multiple spaces with a single space
# +
# convert empty strings to NA
trim_and_clean_internal_ <- function(x) {
  str_replace_all(str_trim(str_replace_all(x, "\\s+", " ")), "^$", NA_character_)
}

trim_and_clean_ <- function(x) {
  lapply(x, function(dt) {
    char_cols <- names(dt)[vapply(dt, is.character, logical(1))]
    dt[, (char_cols) := lapply(.SD, trim_and_clean_internal_),
      .SDcols = char_cols]
  })
}

harmonize_types_ <- function(x) {
  description_elements <- grep("_labels_", names(x), value = TRUE)

  for (element in description_elements) {
    entity <- gsub("_labels_.*", "", element)

    common_col <- intersect(colnames(x[[entity]]), colnames(x[[element]]))

    # if the data type of the column in the entity is different from the
    # description, convert it in the description
    for (col in common_col) {
      entity_col_class <- class(x[[entity]][[col]])
      element_col_class <- class(x[[element]][[col]])

      if (entity_col_class != element_col_class) {
        x[[element]][, (col) := switch(entity_col_class,
          character = as.character(.SD[[col]]),
          factor = as.factor(.SD[[col]]),
          numeric = as.numeric(.SD[[col]]),
          integer = as.integer(.SD[[col]])
        ), .SDcols = col]
      }
    }
  }

  x
}

merge_descriptions_ <- function(x) {
  description_elements <- grep("_labels_", names(x), value = TRUE)

  for (element in description_elements) {
    entity <- gsub("_labels_.*", "", element)

    common_col <- intersect(colnames(x[[entity]]), colnames(x[[element]]))
    noncommon_col <- setdiff(colnames(x[[element]]), common_col)

    # convert to factor for non-common columns
    x[[element]][, (noncommon_col) := lapply(.SD, as.factor),
      .SDcols = noncommon_col]

    # convert any other character column to factor
    char_cols <- names(x[[element]])[vapply(
      x[[element]], is.character, logical(1))]
    x[[element]][, (char_cols) := lapply(.SD, as.factor), .SDcols = char_cols]

    char_cols <- names(x[[entity]])[vapply(
      x[[entity]], is.character, logical(1))]
    x[[entity]][, (char_cols) := lapply(.SD, as.factor), .SDcols = char_cols]
  }

  x
}

datatable_to_tibble_ <- function(x) {
  lapply(x, as_tibble)
}
