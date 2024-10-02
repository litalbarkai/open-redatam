detect_encoding_ <- function(strings) {
  valid_strings <- paste(strings[!is.na(strings) & strings != ""],
    collapse = " ")
  if (length(valid_strings) > 0) {
    detected_encoding <- stri_enc_detect(valid_strings)
    if (length(detected_encoding) > 0) {
      return(detected_encoding[[1]]$Encoding[1])
    }
  }
  return("UTF-8")
}

list_to_datatable_ <- function(x) {
  for (y in names(x)) {
    if (is.list(x[[y]])) {
      x[[y]] <- as.data.table(x[[y]])
    }
  }
  return(x)
}

fix_encoding_recursive_ <- function(x, encoding) {
  if (is.list(x)) {
    return(lapply(x, fix_encoding_recursive_, encoding))
  } else if (is.character(x)) {
    y <- iconv(x, from = encoding, to = "UTF-8", sub = "")
    y <- gsub("\u00c3\u00a1", "\u00e1", y) # a with acute accent
    y <- gsub("\u00c3\u00a9", "\u00e9", y) # e with acute accent
    y <- gsub("\u00c3\u00b3", "\u00f3", y) # o with acute accent
    y <- gsub("\u00c3\u00ba", "\u00fa", y) # u with acute accent
    y <- gsub("\u00c3\u00b1", "\u00f1", y) # n with tilde
    y <- gsub("\u00b1", "\u00f1", y) # n with tilde
    y <- gsub("\u00c3", "\u00ed", y) # i with acute accent
    return(y)
  } else {
    return(x)
  }
}

fix_encoding_ <- function(x) {
  # Extract the unique values of the "chr" vectors in the lists
  strings <- unique(unlist(lapply(x, function(y) {
    if (is.data.table(y)) {
      return(unlist(y[, lapply(
        .SD,
        function(col) {
          if (is.character(col)) col else NULL
        }
      )]))
    } else {
      return(y[vapply(y, is.character, logical(1))])
    }
  })))

  # Detect the encoding of the strings
  encoding <- detect_encoding_(strings)

  # Apply the encoding fix recursively
  return(lapply(x, function(y) {
    if (is.data.table(y)) {
      char_cols <- names(y)[vapply(y, is.character, logical(1))]
      y[, (char_cols) := lapply(.SD,
        function(col) {
          fix_encoding_recursive_(col, encoding)
        }),
        .SDcols = char_cols]
      return(y)
    } else {
      return(fix_encoding_recursive_(y, encoding))
    }
  }))
}

tidy_names_ <- function(x) {
  if (is.list(x)) {
    element_names <- names(x)

    # check for NULL or empty names and replace them with temporary placeholders
    if (is.null(element_names)) {
      element_names <- rep("", length(x))
    }

    # apply janitor to the non-empty names
    cleaned_names <- ifelse(element_names == "", element_names,
      make_clean_names(element_names))

    names(x) <- cleaned_names

    # apply tidy_names_ recursively
    return(lapply(x, function(y) {
      if (is.data.table(y)) {
        setnames(y, make_clean_names(names(y)))
        return(y)
      } else {
        return(tidy_names_(y))
      }
    }))
  } else {
    return(x)
  }
}

# trim leading/trailing spaces and replace multiple spaces with a single space
# +
# convert empty strings to NA
trim_and_clean_internal_ <- function(x) {
  x <- trimws(gsub("^\\s+|\\s+$", "", gsub("\\s+", " ", x)))
  x[x == ""] <- NA
  return(x)
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
      target_class <- class(x[[entity]][[col]])
      if (target_class != class(x[[element]][[col]])) {
        x[[element]][, (col) := switch(target_class,
          character = as.character(.SD[[col]]),
          factor = as.factor(.SD[[col]]),
          numeric = as.numeric(.SD[[col]]),
          integer = as.integer(.SD[[col]])
        ), .SDcols = col]
      }
    }
  }

  return(x)
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

  return(x)
}

datatable_to_tibble_ <- function(x) {
  for (y in names(x)) {
    x[[y]] <- as.data.frame(x[[y]])
    class(x[[y]]) <- c("tbl_df", "tbl", "data.frame")
  }
  return(x)
}
